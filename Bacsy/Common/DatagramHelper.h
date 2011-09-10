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

#ifndef BACSY_DATAGRAM_HELPER_H
#define BACSY_DATAGRAM_HELPER_H

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Exception.h"
#include "Bacsy/Common/Info.h"
#include "Bacsy/Common/JsonHelper.h"

namespace Bacsy
{
namespace Common
{

class DatagramHelper
{
	public:
		template<size_t maxSize, typename FUNCTION>
		static void receiveMessages(
				Poco::Net::DatagramSocket& socket,
				const unsigned int timeoutInMs,
				FUNCTION& exec)
		{
			if(timeoutInMs > 0)
				socket.setReceiveTimeout(Poco::Timespan(0,1000*timeoutInMs));

			const size_t maxsize = maxSize+1;
			char buffer[maxsize];
			try
			{
				while(true)
				{
					Poco::Net::SocketAddress sender;
					const int n = socket.receiveFrom(buffer, maxsize-1, sender);
					buffer[n] = 0;
					if(!n)
					{
						continue;
					}

					exec(sender, buffer);
				}
			}
			catch(Poco::TimeoutException& e)
			{
				// Stop trying to receive messages
				return;
			}
		}

		static std::string toMessage(Json::Value& root)
		{
			return bacsyProtocolString + "\n" + JsonHelper::write(root);
		}

		static std::string toMessageNL(Json::Value& root)
		{
			return toMessage(root) + "\n";
		}
};

}
}
#endif
