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

#ifndef BACSY_READYTOSTOREMESSAGE_H
#define BACSY_READYTOSTOREMESSAGE_H

#include "Bacsy/Messages/AMessage.h"

namespace Bacsy
{
namespace Messages
{

class ReadyToStoreMessage : public AMessage
{
public:
	ReadyToStoreMessage(const std::string& source);

	ReadyToStoreMessage(const Json::Value& root);
	void buildJson(Json::Value& root) const;

	const std::string getSource() const { return source; };
private:
	const std::string source;
};


}
}
#endif
