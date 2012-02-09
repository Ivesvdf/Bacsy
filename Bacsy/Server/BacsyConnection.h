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

#ifndef BACSY_BACSYCONNECTION_H
#define BACSY_BACSYCONNECTION_H

#include <list>
#include <string>
#include "Poco/Timestamp.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Bacsy/Server/StoreManager.h"
#include "Bacsy/Messages/StoreMessage.h"
#include "Bacsy/Common/RunType.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class BacsyConnection : public Poco::Net::TCPServerConnection
{
public:
	BacsyConnection(const Poco::Net::StreamSocket& socket, StoreManager& manager);
	void storeBackup(
			Poco::Net::DialogSocket& ds,
			const Bacsy::Messages::StoreMessage& message);

	void storeNonDeltaInStores(
			Poco::Net::DialogSocket& ds,
			const Bacsy::Messages::StoreMessage& message,
			std::list<Store*> storeTo,
			const std::string ancestor,
			RunType& runType);

	void swapContents(Poco::File walkPath, Poco::File otherPath);
	void swap(Poco::File walkPath, Poco::File otherPath);

	virtual void run();

private:
	StoreManager& storeManager;
};

}
}
#endif
