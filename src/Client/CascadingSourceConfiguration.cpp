/*
 * Copyright (C) 2011  Ives van der Flaas
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
#include "Common/StringUtils.h"
#include "Client/CascadingSourceConfiguration.h"
#include "Rules/StringExclusionRuleBuilder.h"

namespace bacsy
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

std::vector<std::string> CascadingSourceConfiguration::getIncludes(const std::string& source) const 
{
	return StringUtils::split(getCascadingSourceValue<std::string>(
				source,
				"Include"), 
			'\n'); 
}

std::list<ExclusionRule> CascadingSourceConfiguration::getExcludes(const std::string& source) const 
{
	std::list<ExclusionRule> rv;
	const std::vector<std::string> stringExcludes = StringUtils::split(getCascadingSourceValue<std::string>(
				source,
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

unsigned int CascadingSourceConfiguration::getPriority(const std::string& source) const 
{
	return getCascadingSourceValue<unsigned int>(
			source,
			"Priority",
			5);
}

unsigned int CascadingSourceConfiguration::getMinBackups(const std::string& source) const
{
	return getCascadingSourceValue<unsigned int>(
			source,
			"MinBackups",
			1);
}

unsigned int CascadingSourceConfiguration::getMaxBackups(const std::string& source) const
{
	return getCascadingSourceValue<unsigned int>(
			source,
			"MaxBackups",
			std::numeric_limits<unsigned int>::max());
}

bool CascadingSourceConfiguration::getDryPrintRun(const std::string& source) const
{
	return toBool(getCascadingSourceValue<std::string>(
			source,
			"DryPrintRun",
			"False"));
}

bool CascadingSourceConfiguration::getEnabled(const std::string& source) const
{
	return toBool(getCascadingSourceValue<std::string>(
			source,
			"Enabled",
			"True"));
}

std::string CascadingSourceConfiguration::getPreferredOrder(const std::string& source) const
{
	return getCascadingSourceValue<std::string>(
			source,
			"PreferredOrder",
			"this, other");
}

std::string CascadingSourceConfiguration::getDistribution(const std::string& source) const
{
	return getCascadingSourceValue<std::string>(
			source,
			"Distribution",
			"focus");
}

std::string CascadingSourceConfiguration::getTimerString(const std::string& source) const
{
	return getCascadingSourceValue<std::string>(
			source,
			"ExecuteAt",
			"at start");
}


std::string CascadingSourceConfiguration::getHostIdentification(const std::string& source) const
{
	return getCascadingSourceValue<std::string>(
			source,
			"HostIdentification",
			Poco::Environment::nodeName());
}

}
