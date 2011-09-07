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

#ifndef STOREMESSAGE_H
#define STOREMESSAGE_H

#include "json/json.h"
#include "Poco/Timestamp.h"
#include "Bacsy/Messages/AMessage.h"

namespace Bacsy
{
namespace Messages
{

class StoreMessage : AMessage
{
	StoreMessage(const std::string& hostIdentification,
			const std::string& sourceName,
			const unsigned int priority,
			const unsigned int maxStoreTimes);

	StoreMessage(const Json::Value& root);

	const std::string getHostIdentification() const { return hostIdentification; }
	const std::string getSourceName() const { return sourceName; }
	const unsigned int getPriority() const { return priority; }
	const Poco::Timestamp getTime() const { return time; }
	const std::string getRunID() const { return runID; }
	const unsigned int getMaxStoreTimes() const { return maxStoreTimes; }
private:
	const std::string hostIdentification;
	const std::string sourceName;
	const unsigned int priority;
	const Poco::Timestamp time;
	const std::string runID;
	const unsigned int maxStoreTimes;
};


}
}
#endif
