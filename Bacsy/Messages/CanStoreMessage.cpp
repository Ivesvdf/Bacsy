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
#include "Bacsy/Messages/CanStoreMessage.h"

namespace Bacsy
{
namespace Messages
{

CanStoreMessage::CanStoreMessage(const std::string& source,
			const unsigned int priority)
	: 
		AMessage("canStore"),
		source(source),
		priority(priority)
{


}

CanStoreMessage::CanStoreMessage(const Json::Value& root):
	AMessage(root["type"].asString()),
	source(root["source"].asString()),
	priority(root["priority"].asUInt())
{
	
}

void CanStoreMessage::buildJson(Json::Value& root) const
{
	root["source"] = source;
	root["priority"] = priority;
}

}
}
