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

const ISourceConfiguration& CascadingSourceConfiguration::getSource(const std::string& name) const
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

CascadingSourceConfiguration::Section::Section(const std::string& name,
                                      const CascadingSourceConfiguration& config):
    name(name),
	sourceFile(config)
{
}

std::string CascadingSourceConfiguration::Section::getName() const
{
	return name;
}

std::vector<std::string> CascadingSourceConfiguration::Section::getIncludes() const
{
	return StringUtils::split(sourceFile.getCascadingValue<std::string>(
				name,
				"Include"), 
			'\n'); 
}

std::list<ExclusionRule> CascadingSourceConfiguration::Section::getExcludes() const
{
	std::list<ExclusionRule> rv;
	const std::vector<std::string> stringExcludes = StringUtils::split(sourceFile.getCascadingValue<std::string>(
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
	return sourceFile.getCascadingValue<unsigned int>(
			name,
			"Priority",
			5);
}

unsigned int CascadingSourceConfiguration::Section::getMinBackups() const
{
	return sourceFile.getCascadingValue<unsigned int>(
			name,
			"MinBackups",
			1);
}

unsigned int CascadingSourceConfiguration::Section::getMaxBackups() const
{
	return sourceFile.getCascadingValue<unsigned int>(
			name,
			"MaxBackups",
			std::numeric_limits<unsigned int>::max());
}

bool CascadingSourceConfiguration::Section::getDryPrintRun() const
{
	return sourceFile.toBool(sourceFile.getCascadingValue<std::string>(
			name,
			"DryPrintRun",
			"False"));
}

bool CascadingSourceConfiguration::Section::getEnabled() const
{
	return toBool(sourceFile.getCascadingValue<std::string>(
			name,
			"Enabled",
			"True"));
}

ISourceConfiguration::PreferredOrder CascadingSourceConfiguration::Section::getPreferredOrder() const
{
	ISourceConfiguration::PreferredOrder preferred_order;
	preferred_order.push_back(ISourceConfiguration::PREFER_OTHER);
	preferred_order.push_back(ISourceConfiguration::PREFER_THIS);
	return preferred_order;
}

ISourceConfiguration::Distribution CascadingSourceConfiguration::Section::getDistribution() const
{
	std::string distribution = sourceFile.getCascadingValue<std::string>(
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
	std::string timerString = sourceFile.getCascadingValue<std::string>(
			name,
			"ExecuteAt",
			"at start");
	TimerStringParser parser;
	return parser.parse(Poco::LocalDateTime(), timerString);
}


std::string CascadingSourceConfiguration::Section::getHostIdentification() const
{
	return sourceFile.getCascadingValue<std::string>(
			name,
			"HostIdentification",
			Poco::Environment::nodeName());
}

}
