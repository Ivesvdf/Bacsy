/*
 * Copyright (C) 2012  Ives van der Flaas
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

#include <limits>
#include <utility>
#include "Poco/RegularExpression.h"
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Server/StoreTimeParser.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

unsigned int StoreTimeParser::parse(std::string timerString)
{
	Poco::RegularExpression validator("[0-9]+ (day|month|year)s?");
	timerString = StringUtils::toLower(timerString);

	if(!validator.match(timerString))
	{
		LOGE("Invalid StoreTime = " + timerString + ", falling back to +INFINITY");
		return std::numeric_limits<unsigned int>::max();
	}

	const std::vector<std::string> splitString =
		StringUtils::split(timerString, " ");

	unsigned int number = StringUtils::fromString<unsigned int>(splitString[0]);

	const unsigned int secondsInDay = 60*60*24;

	// No reason to start comparing strings, first letters of day, month, year
	// differ
	switch(splitString[1][0])
	{
		case 'd':
			return number*secondsInDay;
		case 'm':
			return number*31*secondsInDay;
		case 'y':
			return number*366*secondsInDay;
	}

	// Avoid compiler warning
	return std::numeric_limits<unsigned int>::max();
}

}
}
