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

#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "stringUtils.h"
#include "info.h"
#include "datagramHelper.h"
#include "multicastResponder.h"

void MulticastResponder::start()
{
	thread.start(*this);
}

void MulticastResponder::respondToMulticast(Poco::Net::SocketAddress address, const std::string& what)
{
	std::vector<std::string> parts = StringUtils::split(what, "\n");

	if(parts.size() != 2 || parts[0] != bacsyProtocolString)
	{
		LOGW( "Received packet with incorrect protocol string or invalid contents:"
				+ what);
		return;
	}

	std::stringstream stream;
	stream << parts[1];
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(stream, root))
	{
		LOGW("Could not parse " + what);
		return;
	}	

	Poco::Net::DatagramSocket responseSocket(Poco::Net::SocketAddress(Poco::Net::IPAddress(), MULTICASTRESPONSEPORT), true);

	if(root["type"] == "canStore")
	{
		LOGI("Received canStore message -- checking if we can store.");
		Json::Value responseRoot;
		responseRoot["type"] = "readyToStore";
		responseRoot["target"] = root["target"];

		const std::string msg = DatagramHelper::toMessage(responseRoot);

		responseSocket.sendTo(msg.c_str(), msg.length(), Poco::Net::SocketAddress(address.host(), MULTICASTPORT));
	}
}

void MulticastResponder::run()
{
	Poco::Net::SocketAddress address(MULTICASTGROUP, MULTICASTPORT);

	Poco::Net::MulticastSocket mcSocket(
			Poco::Net::SocketAddress(
				Poco::Net::IPAddress(), address.port()),
			true // reuse address
			);

	// to receive any data you must join
	LOGI("Joining multicast group " + MULTICASTGROUP);
	mcSocket.joinGroup(address.host());
	LOGI("Joined multicast group.");
	
	DatagramHelper::receiveMessages<512>(mcSocket, 0, MulticastResponder::respondToMulticast);
}
