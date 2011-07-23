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
#include "stringUtils.h"
#include "cascadingFileConfiguration.h"

CascadingFileConfiguration::CascadingFileConfiguration(const std::string& directory):
	inputTargetStream((StringUtils::rstrip(directory, "/") + std::string("/targets.config")).c_str()),
	targetConfig(inputTargetStream),
	globalSectionName("global")

{

}

std::list<std::string> CascadingFileConfiguration::getTargets() const
{
	std::list<std::string> targetNames = targetConfig.sections();
	targetNames.remove_if(std::bind2nd(std::equal_to<std::string>(), "global"));
	return targetNames;
}

std::vector<std::string> CascadingFileConfiguration::getIncludes(const std::string& target) const 
{
	return StringUtils::split(getCascadingValue<std::string>(
				target,
				"Include"), 
			'\n'); 
}

std::vector<std::string> CascadingFileConfiguration::getExcludes(const std::string& target) const 
{
	return StringUtils::split(getCascadingValue<std::string>(
				target,
				"Exclude"), 
			'\n'); 
}

unsigned int CascadingFileConfiguration::getPriority(const std::string& target) const 
{
	return getCascadingValue<unsigned int>(
			target,
			"Priority",
			5);
}

unsigned int CascadingFileConfiguration::getMinBackups(const std::string& target) const
{
	return getCascadingValue<unsigned int>(
			target,
			"MinBackups",
			1);
}

unsigned int CascadingFileConfiguration::getMaxBackups(const std::string& target) const
{
	return getCascadingValue<unsigned int>(
			target,
			"MaxBackups",
			std::numeric_limits<unsigned int>::max());
}

std::string CascadingFileConfiguration::getPreferredOrder(const std::string& target) const
{
	return getCascadingValue<std::string>(
			target,
			"PreferredOrder",
			"this, other");
}

std::string CascadingFileConfiguration::getDistribution(const std::string& target) const
{
	return getCascadingValue<std::string>(
			target,
			"Distribution",
			"focus");
}

std::string CascadingFileConfiguration::getTimerString(const std::string& target) const
{
	return getCascadingValue<std::string>(
			target,
			"ExecuteAt",
			"at start");
}
