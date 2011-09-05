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

#include "Poco/DateTimeFormatter.h"
#include "Bacsy/Common/VersionsType.h"
#include "Bacsy/Common/StringUtils.h"

namespace Bacsy
{
namespace Common
{
VersionsType::VersionsType(): versions(0), time(0) {}

void VersionsType::setVersions(unsigned int inputVersions)
{
	versions = inputVersions;
}

void VersionsType::setTime(Poco::Timespan inputTime)
{
	time = inputTime;
}

unsigned int VersionsType::getVersions() const 
{
	return versions; 
}

Poco::Timespan VersionsType::getTime() const 
{
	return time; 
}

bool VersionsType::versionsIsSet() const 
{
	return versions != 0; 
}

bool VersionsType::timeIsSet() const 
{
	return time != 0; 
}

std::string VersionsType::toString() const
{
	std::string rv;
	if(versionsIsSet())
	{
		rv += StringUtils::toString(versions) + " versions";
	}

	if(versionsIsSet() && timeIsSet())
	{
		rv += " and ";
	}

	if(timeIsSet())
	{
		rv += Poco::DateTimeFormatter::format(time, "%d days");
	}

	return rv;
}

}
}
