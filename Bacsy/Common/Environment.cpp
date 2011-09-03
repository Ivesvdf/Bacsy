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

#include "Poco/Foundation.h"
#include "Poco/Environment.h"
#include "Bacsy/Common/StringUtils.h"
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/Environment.h"

namespace Bacsy
{
namespace Common
{

std::string Environment::getDefaultConfigurationDirectory()
{
	std::string prefix;
#if defined(POCO_OS_FAMILY_WINDOWS)
	if(Poco::Environment::has("APPDATA"))
	{
		prefix = StringUtils::rstrip(Poco::Environment::get("APPDATA") , "\\/") + "/Bacsy/";
	}
	else // No idea why APPDATA wouldn't be defined
	{
		LOGE("%APPDATA% is not defined -- falling back to C:/Bacsy");
		prefix = "C:/Bacsy/";
	}
#elif defined(POCO_OS_FAMILY_UNIX)
	const std::string homeConfig = 
		Poco::Environment::has("XDG_CONFIG_HOME") 
			? Poco::Environment::get("XDG_CONFIG_HOME") 
			: StringUtils::rstrip(Poco::Environment::get("HOME"), "/") + "/.config";

	prefix = StringUtils::rstrip(homeConfig, "/") + "/bacsy/";
#endif

	return prefix;
}

}
}
