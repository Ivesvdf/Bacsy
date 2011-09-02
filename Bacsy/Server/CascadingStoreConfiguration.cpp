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
#include "Bacsy/Server/CascadingStoreConfiguration.h"
#include "Bacsy/Rules/StringExclusionRuleBuilder.h"

namespace Bacsy
{

CascadingStoreConfiguration::CascadingStoreConfiguration(const std::string& directory):
	CascadingFileConfiguration((StringUtils::rstrip(directory, "/") + std::string("/stores.config")))
{

}

std::list<std::string> CascadingStoreConfiguration::getStores() const
{
	std::list<std::string> storeNames = getConfig().sections();
	storeNames.remove_if(std::bind2nd(std::equal_to<std::string>(), globalSectionName));
	return storeNames;
}

CascadingStoreConfiguration::Section::Section(const std::string& name,
                                     const CascadingStoreConfiguration& config):
 name(name),
 storeConfig(config)
{
}

const IStoreConfiguration& CascadingStoreConfiguration::getStore(const std::string& name) const
{
	if (config.hasSection(name))
	{
		return Section(name, *this);
	}
	else
	{
		throw NoSuchStoreException(name);
	}
}

std::string CascadingStoreConfiguration::Section::getName() const
{
	return name;
}

std::string CascadingStoreConfiguration::Section::getLocation() const
{
	return storeConfig.getCascadingStoreValue<std::string>(
			name,
			"Location");
}

bool CascadingStoreConfiguration::Section::getAlwaysPresent() const
{
	return toBool(storeConfig.getCascadingStoreValue<std::string>(
			name,
			"AlwaysPresent",
			"True"));
}

unsigned int CascadingStoreConfiguration::Section::getMinPriorityForStoring() const
{
	return storeConfig.getCascadingStoreValue<unsigned int>(
			name,
			"MinPriorityForStoring",
			10);
}


}
