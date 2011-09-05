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

ISourceConfiguration::MinVersions CascadingSourceConfiguration::Section::getMinVersions() const
{
	return getVersions("MinVersions");
}

ISourceConfiguration::MaxVersions CascadingSourceConfiguration::Section::getMaxVersions() const
{
	return getVersions("MaxVersions");
}

ISourceConfiguration::VersionsType CascadingSourceConfiguration::Section::getVersions(const std::string& key) const
{
	const std::string stringValue = sourceFile.getCascadingValue<std::string>(
				name,
				key);

	std::vector<std::string> parts =  StringUtils::split(stringValue, ' '); 
	std::transform(parts.begin(), parts.end(), parts.begin(), StringUtils::toLower);

	const size_t count = parts.size();

	if((count != 2 && count != 5) || (count == 5 && (parts[2] != "and" && parts[2] != "or")))
	{
		throw std::runtime_error("Invalid number of parts in " + key + " string: " + stringValue);
	}

	const size_t segments = (count == 2) ? 1 : 2;
	VersionsType rv;

	for(size_t i = 0; i < segments; i++)
	{
		size_t offset = (i*3);

		const std::string& first = parts[offset];
		const std::string& second = parts[offset+1];

		if(!StringUtils::isInteger(first))
		{
			throw std::runtime_error("Invalid integer " + first + " in VersionSpec " + stringValue);
		}

		const size_t number = StringUtils::fromString<size_t>(first);

		if(second == "versions")
		{
			if(rv.versionsIsSet())
				throw std::runtime_error("Versions is already set in VersionSpec " + stringValue);

			rv.setVersions(number);
		}
		else
		{
			Poco::Timespan time;
			if(second == "hours")
			{
				time = number*Poco::Timespan::HOURS;
			}
			else if(second == "days")
			{
				time = number*Poco::Timespan::DAYS;
			}
			else if(second == "months")
			{
				time = number*30*Poco::Timespan::DAYS;
			}
			else if(second == "years")
			{
				time = number*365*Poco::Timespan::DAYS;
			}
			else
			{
				throw std::runtime_error("Invalid qualifier " + second + " in VersionSpec " + stringValue);
			}

			if(rv.timeIsSet())
				throw std::runtime_error("Time is already set in VersionSpec " + stringValue);

			rv.setTime(time);
		}
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
