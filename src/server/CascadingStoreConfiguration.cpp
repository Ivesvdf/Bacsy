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
#include "common/StringUtils.h"
#include "server/CascadingStoreConfiguration.h"
#include "rules/StringExclusionRuleBuilder.h"

namespace bacsy
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

std::string CascadingStoreConfiguration::getLocation(const std::string& store) const
{
	return getCascadingStoreValue<std::string>(
			store,
			"Location");
}

bool CascadingStoreConfiguration::getAlwaysPresent(const std::string& store) const
{
	return toBool(getCascadingStoreValue<std::string>(
			store,
			"AlwaysPresent",
			"True"));
}

unsigned int CascadingStoreConfiguration::getMinPriorityForStoring(const std::string& store) const
{
	return getCascadingStoreValue<unsigned int>(
			store,
			"MinPriorityForStoring",
			10);
}


}
