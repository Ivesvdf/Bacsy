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
#include "stringUtils.h"
#include "cascadingFileConfiguration.h"
#include "stringExclusionRuleBuilder.h"

CascadingFileConfiguration::CascadingFileConfiguration(const std::string& directory):
	inputTargetStream((StringUtils::rstrip(directory, "/") + std::string("/targets.config")).c_str()),
	inputStoreStream((StringUtils::rstrip(directory, "/") + std::string("/stores.config")).c_str()),
	targetConfig(inputTargetStream),
	storeConfig(inputStoreStream),
	globalSectionName("global")

{

}

std::list<std::string> CascadingFileConfiguration::getTargets() const
{
	std::list<std::string> targetNames = targetConfig.sections();
	targetNames.remove_if(std::bind2nd(std::equal_to<std::string>(), "global"));
	return targetNames;
}

std::list<std::string> CascadingFileConfiguration::getStores() const
{
	std::list<std::string> storeNames = storeConfig.sections();
	storeNames.remove_if(std::bind2nd(std::equal_to<std::string>(), "global"));
	return storeNames;
}

std::vector<std::string> CascadingFileConfiguration::getIncludes(const std::string& target) const 
{
	return StringUtils::split(getCascadingTargetValue<std::string>(
				target,
				"Include"), 
			'\n'); 
}

std::list<ExclusionRule> CascadingFileConfiguration::getExcludes(const std::string& target) const 
{
	std::list<ExclusionRule> rv;
	const std::vector<std::string> stringExcludes = StringUtils::split(getCascadingTargetValue<std::string>(
				target,
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

unsigned int CascadingFileConfiguration::getPriority(const std::string& target) const 
{
	return getCascadingTargetValue<unsigned int>(
			target,
			"Priority",
			5);
}

unsigned int CascadingFileConfiguration::getMinBackups(const std::string& target) const
{
	return getCascadingTargetValue<unsigned int>(
			target,
			"MinBackups",
			1);
}

unsigned int CascadingFileConfiguration::getMaxBackups(const std::string& target) const
{
	return getCascadingTargetValue<unsigned int>(
			target,
			"MaxBackups",
			std::numeric_limits<unsigned int>::max());
}

std::string CascadingFileConfiguration::getPreferredOrder(const std::string& target) const
{
	return getCascadingTargetValue<std::string>(
			target,
			"PreferredOrder",
			"this, other");
}

std::string CascadingFileConfiguration::getDistribution(const std::string& target) const
{
	return getCascadingTargetValue<std::string>(
			target,
			"Distribution",
			"focus");
}

std::string CascadingFileConfiguration::getTimerString(const std::string& target) const
{
	return getCascadingTargetValue<std::string>(
			target,
			"ExecuteAt",
			"at start");
}


std::string CascadingFileConfiguration::getHostIdentification(const std::string& target) const
{
	return getCascadingTargetValue<std::string>(
			target,
			"HostIdentification",
			Poco::Environment::nodeName());
}

std::string CascadingFileConfiguration::getLocation(const std::string& store) const
{
	return getCascadingStoreValue<std::string>(
			store,
			"Location");
}

bool CascadingFileConfiguration::getAlwaysPresent(const std::string& store) const
{
	return toBool(getCascadingStoreValue<std::string>(
			store,
			"AlwaysPresent",
			"True"));
}

unsigned int CascadingFileConfiguration::getMinPriorityForStoring(const std::string& store) const
{
	return getCascadingStoreValue<unsigned int>(
			store,
			"MinPriorityForStoring",
			10);
}

bool CascadingFileConfiguration::toBool(const std::string& input)
{
	const std::string lowered = Poco::toLower(input);

	return (lowered == "true" || lowered == "1");
}


