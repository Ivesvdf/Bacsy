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
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Common/CascadingFileConfiguration.h"
#include "Bacsy/Rules/StringExclusionRuleBuilder.h"

namespace Bacsy
{
namespace Common
{

CascadingFileConfiguration::CascadingFileConfiguration(const std::string& filename):
	inputStream(filename.c_str()),
	configIsLoaded(inputStream),
	config(inputStream),
	globalSectionName("global")
{
}

void CascadingFileConfiguration::checkKeys() const
{
	std::set<std::string> validKeys = getValidKeys();
	std::set<std::string> invalidKeys;

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
			"Unknown key(s) in configuration: "
			+ StringUtils::join(invalidKeys.begin(), invalidKeys.end(), ", "));
	}
}


bool CascadingFileConfiguration::toBool(const std::string& input)
{
	const std::string lowered = Poco::toLower(input);

	return (lowered == "true" || lowered == "1");
}

const ConfigurationFile& CascadingFileConfiguration::getConfig() const
{
	return config;
}

ConfigurationFile& CascadingFileConfiguration::getConfig()
{
	return config;
}

bool CascadingFileConfiguration::isLoaded() const
{
	return configIsLoaded;
}

}
}
