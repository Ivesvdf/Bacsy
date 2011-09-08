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
#include "Bacsy/Messages/StoreMessage.h"

namespace Bacsy
{
namespace Messages
{

StoreMessage::StoreMessage(const std::string& hostIdentification,
		const std::string& sourceName,
		const unsigned int priority,
		const unsigned int maxStoreTimes)
	: 
		AMessage("store"),
		hostIdentification(hostIdentification),
		sourceName(sourceName),
		priority(priority),
		time(Poco::Timestamp()),
		runID(Poco::DateTimeFormatter::format(
					time,
					"%Y-%m-%dT%H.%M.%S%z")),
		maxStoreTimes(maxStoreTimes)
{


}

StoreMessage::StoreMessage(const Json::Value& root):
	AMessage(root["type"].asString()),
	hostIdentification(root["host"].asString()),
	sourceName(root["source"].asString()),
	priority(root["priority"].asUInt()),
	time(Poco::Timestamp::fromUtcTime(root["time"].asInt64())),
	runID(root["runID"].asString()),
	maxStoreTimes(root["maxStoreTimes"].asUInt())
{
	
}

void StoreMessage::buildJson(Json::Value& root) const
{
	root["host"] = hostIdentification;
	root["source"] = sourceName;
	root["priority"] = priority;
	root["runID"] = runID;
	root["time"] = time.utcTime();
	root["maxStoreTimes"] = maxStoreTimes;
}

}
}
