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

#include <algorithm>
#include <functional>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Ascii.h"
#include "Poco/FileStream.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Environment.h"
#include "Poco/StreamCopier.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "functional.h"
#include "bacsyConnection.h"
#include "stringUtils.h"
#include "streamUtilities.h"
#include "info.h"
#include "jsonHelper.h"


BacsyConnection::BacsyConnection(const Poco::Net::StreamSocket& socket, StoreManager& manager) : 
	Poco::Net::TCPServerConnection(socket),
	storeManager(manager)
{

}

void BacsyConnection::run()
{
	try
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
		LOGI("Magic string ok");

		std::string json;
		ds.receiveMessage(json);
		Json::Value root = JsonHelper::read(json);

		if(root["type"] == "store")
		{
			storeBackup(
					ds,
					root["host"].asString(),
					root["target"].asString(),
					root["priority"].asUInt(),
					root["runID"].asString());
		}
		else
		{
			LOGE("Unrecognized command: " + json + " -- stopping connection");
			return;
		}
		LOGI("Naturally stopping connection with client");
	}
	catch(std::exception& e)
	{
		LOGE(std::string("Terminal exception: ") + e.what());
	}
	catch(...)
	{
		LOGE("Terminal unknown exception (not an std::exception&).");
	}
}


void BacsyConnection::storeBackup(Poco::Net::DialogSocket& ds, 
		const std::string host,
		const std::string target,
		const unsigned int priority,
		const std::string runID)
{
	LOGI("Storing backup for " + host);

	// Test if there are any stores available
	std::list<Store*> usableStores = storeManager.getStores(priority);

	std::string file;
	std::string size;
	std::string checksum;
	while(ds.receiveMessage(file) && ds.receiveMessage(size)) // && ds.receiveMessage(checksum))
	{
		Poco::Path originalPath(file);

		Poco::Path storePath(Poco::Path::temp());
		storePath.pushDirectory("backup");
		Poco::Path newPath = storePath;
		std::string nodeIdentification(originalPath.getNode());

		newPath.pushDirectory(host);
		newPath.pushDirectory(target + "_" + runID);

		// Keep only alphabetic characters
		nodeIdentification.erase(
				std::remove_if(
					nodeIdentification.begin(),
					nodeIdentification.end(),
					std::not1(fun_ref(Poco::Ascii::isAlpha))),
				nodeIdentification.end());

		if(nodeIdentification.empty())
			nodeIdentification = "root";

		newPath.pushDirectory(nodeIdentification);

		for(int i = 0; i < originalPath.depth(); i++)
		{
			newPath.pushDirectory(originalPath[i]);
		}

		Poco::File newPathFile(newPath);
		if(!newPathFile.exists())
			newPathFile.createDirectories();

		newPath.setFileName(originalPath.getFileName());

		LOGI("Receiving file = " + newPath.toString());
		LOGI("Receiving size = " + size);
		backupFile(ds, newPath.toString(), StringUtils::fromString<size_t>(size), priority);
	}
}

void BacsyConnection::backupFile(
		Poco::Net::DialogSocket& ds,
		std::string targetFile,
		size_t size,
		unsigned int priority)
{
	LOGI("Pushing file " + targetFile + " to the backup queue.");

	Poco::FileOutputStream output;
	try
	{
		output.open(targetFile, std::ios::out | std::ios::trunc);
	}
	catch(Poco::FileException& e)
	{
		LOGE("Cannot store file " + targetFile + " because of exception: " + e.what());
		return;
	}

	// We must keep receiving the file! If not the protocol gets out of sync
	// and we're screwed. 
	std::streamsize received = StreamUtilities::copyStream(ds, output, 65536, size);
	if(received != static_cast<std::streamsize>(size))
	{
		LOGE("Received sizes do not match -- I should do something about this.");
	}
	LOGI("Bytes received = " + StringUtils::toString(received));

	output.close();
}

