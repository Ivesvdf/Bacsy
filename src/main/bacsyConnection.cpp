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

#include "woodcutter/woodcutter.h"
#include "bacsyConnection.h"
#include "stringUtils.h"
#include "info.h"


BacsyConnection::BacsyConnection(const Poco::Net::StreamSocket& socket) : Poco::Net::TCPServerConnection(socket)
{

}

void BacsyConnection::run()
{
	LOGI("New client connected -- starting BacsyConnection");

	Poco::Net::DialogSocket ds(socket());

	std::string magicString;
	ds.receiveMessage(magicString);

	if(magicString != "BACSY " + bacsyVersion)
	{
		LOGE("Invalid magic string; do protocol versions match? -- stopping connection");
		return;
	}

	std::string command;
	ds.receiveMessage(command);

	if(command == "STORE")
	{
		storeBackup(ds);
	}
	else
	{
		LOGE("Unrecognized command: " + command + " -- stopping connection");
		return;
	}
	LOGI("Naturally stopping connection with client");
}


void BacsyConnection::storeBackup(Poco::Net::DialogSocket& ds)
{
	LOGI("Storing backup.");

	std::string hostIdentification;
	ds.receiveMessage(hostIdentification);

	LOGI("Backup is for " + hostIdentification);

	// Test if there are any stores available

	std::string file;
	std::string checksum;
	while(ds.receiveMessage(file) && ds.receiveMessage(checksum))
	{
		backupFile(file);
	}
}

void BacsyConnection::backupFile(std::string file)
{
	LOGI("Pushing file " + file + " to the backup queue.");
}

