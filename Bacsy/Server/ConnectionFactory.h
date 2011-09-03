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

#ifndef CONNECTION_FACTORY_H
#define CONNECTION_FACTORY_H

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Bacsy/Server/BacsyConnection.h"
#include "Bacsy/Server/StoreManager.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class ConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
	ConnectionFactory(StoreManager& manager):
		manager(manager)
	{}

	virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket)
	{
		return new BacsyConnection(socket, manager);
	}	

private:
	StoreManager& manager;
};

}
}
#endif
