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

#ifndef BACSY_MULTICAST_RESPONDER_H
#define BACSY_MULTICAST_RESPONDER_H

#include <string>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Net/SocketAddress.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class MulticastResponder : public Poco::Runnable
{
	public:
		void start();
		virtual void run();

	private:
		Poco::Thread thread;

		static void respondToMulticast(Poco::Net::SocketAddress address, const std::string& what);
};

}
}
#endif
