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

#include "Poco/FileStream.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Environment.h"
#include "Poco/StreamCopier.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "bacsyConnection.h"
#include "stringUtils.h"
#include "streamUtilities.h"
#include "info.h"
#include "jsonHelper.h"


BacsyConnection::BacsyConnection(const Poco::Net::StreamSocket& socket, StoreManager& manager) : 
	Poco::Net::TCPServerConnection(socket)
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

	std::string json;
	ds.receiveMessage(json);
	Json::Value root = JsonHelper::read(json);

	if(root["type"] == "store")
	{
		storeBackup(
				ds,
				root["host"].asString(),
				root["target"].asString());
	}
	else
	{
		LOGE("Unrecognized command: " + json + " -- stopping connection");
		return;
	}
	LOGI("Naturally stopping connection with client");
}


void BacsyConnection::storeBackup(Poco::Net::DialogSocket& ds, 
		const std::string host,
		const std::string target)
{
	LOGI("Storing backup.");

	LOGI("Backup is for " + host);

	// Test if there are any stores available

	std::string file;
	std::string size;
	std::string checksum;
	while(ds.receiveMessage(file) && ds.receiveMessage(size)) // && ds.receiveMessage(checksum))
	{
		LOGI("Receiving file = " + file);
		LOGI("Receiving size = " + size);
		backupFile(ds, file, StringUtils::fromString<size_t>(size));
	}
}

void BacsyConnection::backupFile(
		Poco::Net::DialogSocket& ds,
		std::string filename,
		size_t size)
{
	LOGI("Pushing file " + filename + " to the backup queue.");

	Poco::FileOutputStream output("/tmp/bla.txt");

	std::streamsize received = StreamUtilities::copyStream(ds, output, 65536, size);
	if(received != static_cast<std::streamsize>(size))
	{
		LOGE("Received sizes do not match -- I should do something about this.");
	}
	LOGI("Bytes received = " + StringUtils::toString(received));

	output.close();
}

