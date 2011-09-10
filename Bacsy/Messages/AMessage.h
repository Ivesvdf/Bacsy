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

#ifndef BACSY_AMESSAGE_H
#define BACSY_AMESSAGE_H

#include <string>
#include "json/json.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/DatagramSocket.h"

namespace Bacsy
{
namespace Messages
{

class AMessage
{
public:
	AMessage(const std::string& type);
	virtual ~AMessage();

	virtual void buildJson(Json::Value& root) const = 0;
	void send(Poco::Net::DialogSocket& socket);
	void send(Poco::Net::DatagramSocket& socket, Poco::Net::SocketAddress to);

protected:
	const std::string type;
	static Json::Value fromDialogSocket(Poco::Net::DialogSocket& ds);
};


}
}
#endif
