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

#ifndef MULTICAST_HELPER_H
#define MULTICAST_HELPER_H

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/MulticastSocket.h"

class MulticastHelper
{
	public:
		template<size_t maxSize, typename FUNCTION>
		static void receiveMessages(Poco::Net::MulticastSocket& socket, const unsigned int timeoutInMs, FUNCTION exec)
		{
			socket.setReceiveTimeout(Poco::Timespan(0,timeoutInMs));
			const size_t maxsize = maxSize+1;
			char buffer[maxsize];
			while(true)
			{
				try 
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
				catch(Poco::TimeoutException& e)
				{
					// Stop trying to receive messages
					break;
				}
			}
		}
};

#endif
