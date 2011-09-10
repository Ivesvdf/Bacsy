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

#include "Poco/Net/DialogSocket.h"
#include "Bacsy/Messages/AMessage.h"
#include "Bacsy/Common/JsonHelper.h"
#include "Bacsy/Common/DatagramHelper.h"

namespace Bacsy
{
namespace Messages
{

using Bacsy::Common::JsonHelper;

AMessage::AMessage(const std::string& type)
		: type(type)
{


}

AMessage::~AMessage()
{
}

void AMessage::send(Poco::Net::DialogSocket& socket)
{
	Json::Value root;
	root["type"] = type;
	buildJson(root);
	socket.sendMessage(JsonHelper::write(root));
}

void AMessage::send(Poco::Net::DatagramSocket& socket, Poco::Net::SocketAddress to)
{
	Json::Value root;
	root["type"] = type;
	buildJson(root);

	const std::string msg = Bacsy::Common::DatagramHelper::toMessage(root);
	socket.sendTo(msg.c_str(), msg.size(), to);
}

Json::Value AMessage::fromDialogSocket(Poco::Net::DialogSocket& ds)
{
	std::string json;
	ds.receiveMessage(json);
	Json::Value root = JsonHelper::read(json);

	return root;
}

}
}
