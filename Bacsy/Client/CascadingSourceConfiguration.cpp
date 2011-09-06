/*
 * Copyright (C) 2011  Ives van der Flaas
 * Copyright (C) 2011  Nathan Samson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <set>
#include <functional>
#include <algorithm>
#include "Poco/Environment.h"
#include <Poco/String.h>
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Client/CascadingSourceConfiguration.h"
#include "Bacsy/Client/TimerStringParser.h"
#include "Bacsy/Rules/StringExclusionRuleBuilder.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;
using namespace Rules;

CascadingSourceConfiguration::CascadingSourceConfiguration(const std::string& directory):
	CascadingFileConfiguration((StringUtils::rstrip(directory, "/") + std::string("/sources.config")))
{
	checkKeys();
}

void CascadingSourceConfiguration::checkKeys() const
{
	std::set<std::string> validKeys;
	std::set<std::string> invalidKeys;
	validKeys.insert("Include");
	validKeys.insert("Exclude");
	validKeys.insert("Priority");
	validKeys.insert("MinBackups");
	validKeys.insert("MaxBackups");
	validKeys.insert("DryPrintRun");
	validKeys.insert("Enabled");
	validKeys.insert("PreferredOrder");
	validKeys.insert("Distribution");
	validKeys.insert("ExecuteAt");
	validKeys.insert("HostIdentification");

	std::list<std::string> sections = config.sections();

	for(std::list<std::string>::iterator sectionIt = sections.begin();
			sectionIt != sections.end();
			++sectionIt)
	{
		std::list<std::string> keys = config.keys(*sectionIt);

		for(std::list<std::string>::iterator keyIterator = keys.begin();
				keyIterator != keys.end();
				++keyIterator)
		{
			if(validKeys.count(*keyIterator) == 0)
			{
				invalidKeys.insert(*keyIterator);
			}
		}
	}

	if(invalidKeys.size() > 0)
	{
		throw std::runtime_error(
			"Unknown key(s) in source configuration: " 
			+ StringUtils::join(invalidKeys.begin(), invalidKeys.end(), ", "));
	}
}

std::list<std::string> CascadingSourceConfiguration::getSources() const
{
	std::list<std::string> sourceNames = getConfig().sections();
	sourceNames.remove_if(std::bind2nd(std::equal_to<std::string>(), globalSectionName));
	return sourceNames;
}

const CascadingSourceConfiguration::Section CascadingSourceConfiguration::getSource(const std::string& name) const
{
	if (config.hasSection(name))
	{
		return Section(name, *this);
	}
	else
	{
		throw NoSuchSourceException(name);
	}
}

CascadingSourceConfiguration::Section::Section(
		const std::string& name,
		const CascadingSourceConfiguration& config):
	name(name),
	config(config)
{
}

std::string CascadingSourceConfiguration::Section::getName() const
{
	return name;
}

ISourceConfiguration::IncludeCollection CascadingSourceConfiguration::Section::getIncludes() const
{
	return StringUtils::split(config.getCascadingValue<std::string>(
				name,
				"Include"), 
			'\n'); 
}

ISourceConfiguration::ExcludeCollection CascadingSourceConfiguration::Section::getExcludes() const
{
	ExcludeCollection rv;
	const std::vector<std::string> stringExcludes = StringUtils::split(config.getCascadingValue<std::string>(
				name,
				"Exclude"), 
			'\n'); 

	for(std::vector<std::string>::const_iterator it = stringExcludes.begin();
			it != stringExcludes.end();
			++it)
	{
		rv.push_back(StringExclusionRuleBuilder::build(*it));
	}

	return rv;
}

unsigned int CascadingSourceConfiguration::Section::getPriority() const
{
	return config.getCascadingValue<unsigned int>(
			name,
			"Priority",
			5);
}

unsigned int CascadingSourceConfiguration::Section::getMinBackups() const
{
	return config.getCascadingValue<unsigned int>(
			name,
			"MinBackups",
			1);
}

unsigned int CascadingSourceConfiguration::Section::getMaxBackups() const
{
	return config.getCascadingValue<unsigned int>(
			name,
			"MaxBackups",
			std::numeric_limits<unsigned int>::max());
}

bool CascadingSourceConfiguration::Section::getDryPrintRun() const
{
	return config.toBool(config.getCascadingValue<std::string>(
			name,
			"DryPrintRun",
			"False"));
}

bool CascadingSourceConfiguration::Section::getEnabled() const
{
	return toBool(config.getCascadingValue<std::string>(
			name,
			"Enabled",
			"True"));
}

ISourceConfiguration::PreferredOrder CascadingSourceConfiguration::Section::getPreferredOrder() const
{
	const std::string preferredOrderString = config.getCascadingValue<std::string>(
			name,
			"PreferredOrder",
			"this, other");
	std::vector<std::string> parts = StringUtils::split(preferredOrderString, ',');

	// Strip whitespace
	std::transform(parts.begin(), parts.end(), parts.begin(), StringUtils::Stripper(" "));
	std::transform(parts.begin(), parts.end(), parts.begin(), StringUtils::toLower);

	ISourceConfiguration::PreferredOrder preferredOrder;
	for(std::vector<std::string>::const_iterator it = parts.begin(); 
			it != parts.end();
			++it)
	{
		if(*it == "this")
		{
			preferredOrder.push_back(ISourceConfiguration::PREFER_THIS);
		}
		else if(*it == "other")
		{
			preferredOrder.push_back(ISourceConfiguration::PREFER_OTHER);
		}
		else
		{
			throw std::runtime_error("Unrecognized PreferredOrder part " + *it
					+ " in PreferredOrder " + preferredOrderString);
		}
	}
	return preferredOrder;
}

ISourceConfiguration::Distribution CascadingSourceConfiguration::Section::getDistribution() const
{
	std::string distribution = config.getCascadingValue<std::string>(
			name,
			"Distribution",
			"focus");
	distribution = StringUtils::strip(distribution, " \t");
	if (distribution == "spread")
	{
		return ISourceConfiguration::DISTRIBUTION_SPREAD;
	}
	else if (distribution == "focus")
	{
		return ISourceConfiguration::DISTRIBUTION_FOCUS;
	}
	else
	{
		throw runtime_error("Distribution value is not valid.");
	}
}

ISourceConfiguration::TimeTable CascadingSourceConfiguration::Section::getTimeTable() const
{
	std::string timerString = config.getCascadingValue<std::string>(
			name,
			"ExecuteAt",
			"at start");
	TimerStringParser parser;
	return parser.parse(Poco::LocalDateTime(), timerString);
}


std::string CascadingSourceConfiguration::Section::getHostIdentification() const
{
	return config.getCascadingValue<std::string>(
			name,
			"HostIdentification",
			Poco::Environment::nodeName());
}

}
}
