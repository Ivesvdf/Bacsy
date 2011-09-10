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
#include <utility>
#include <functional>
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Ascii.h"
#include "Poco/FileStream.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Environment.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timestamp.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "Bacsy/Common/Functional.h"
#include "Bacsy/Server/BacsyConnection.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Streams/StreamUtils.h"
#include "Bacsy/Common/Info.h"
#include "Bacsy/Common/JsonHelper.h"
#include "Bacsy/Messages/StoreResponseMessage.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;
using namespace Streams;


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
			storeBackup(ds, Bacsy::Messages::StoreMessage(root));
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
		const Bacsy::Messages::StoreMessage& message)
{
	LOGI("Storing backup for " + message.getHostIdentification());

	// Test if there are any stores available
	typedef std::list<Store*> StorePointerList;
	StorePointerList storesToTry = storeManager.getStores(message.getPriority());

	if(storesToTry.empty())
	{
		LOGW("No stores found with a MinPriorityForStoring higher than "
				+ StringUtils::toString(message.getPriority())
				+ ", cannot possibly accept backup.");
		return;
	}

	unsigned int storesSentTo = 0;

	while(message.getMaxStoreTimes() - storesSentTo > 0 && !storesToTry.empty())
	{
		LOGI("Entering");
		StorePointerList sendTo;

		Store& store = *(*storesToTry.begin());

		// Find the ancestor for this store, we'll later on find all stores
		// that share this ancestor and give them priority (it saves
		// bandwidth)
		Store::NewRunSpecification spec = store.getNewRunSpecification(
				message.getHostIdentification(),
				message.getSourceName());

		unsigned int nrAdded = 0;

		for(StorePointerList::iterator it = storesToTry.begin();
				it != storesToTry.end();
				++it)
		{
			Store::NewRunSpecification otherSpec
				= (*it)->getNewRunSpecification(
						message.getHostIdentification(),
						message.getSourceName());

			if(spec.runType == otherSpec.runType
					&& spec.ancestorDirectory == otherSpec.ancestorDirectory
					&& nrAdded < message.getMaxStoreTimes() - storesSentTo)
			{
				sendTo.push_back(*it);
				nrAdded++;
			}
		}

		Bacsy::Messages::StoreResponseMessage responseMessage(spec.runType);
		responseMessage.send(ds);

		LOGI("Storing in stores: ");
		for(StorePointerList::const_iterator it = storesToTry.begin();
				it != storesToTry.end();
				++it)
		{
			LOGI("  - " + (*it)->toString());
		}

		if(spec.runType == RunType::full
				|| spec.runType == RunType::fullfiles)
		{
			storeNonDeltaInStores(
					ds,
					message,
					sendTo,
					spec.ancestorDirectory,
					spec.runType);
		}
		else
		{
			LOGE("Fullfiles run -- not implemented.");
			// Not yet implemented
		}

		storesSentTo += sendTo.size();

		for(StorePointerList::iterator it = sendTo.begin();
				it != sendTo.end();
				it++)
		{
			storesToTry.remove(*it);
		}
	}
	
}

class FileAssociations
{
public:
	FileAssociations(Poco::FileOutputStream* fileOutputStream, SimpleOStreamStream* ostreamStream, Poco::File file):
		fileOutputStream(fileOutputStream),
		ostreamStream(ostreamStream),
		file(file)
	{
	}

	Poco::FileOutputStream* fileOutputStream;
	SimpleOStreamStream* ostreamStream;
	Poco::File file;
};

void BacsyConnection::storeNonDeltaInStores(
		Poco::Net::DialogSocket& ds,
		const Bacsy::Messages::StoreMessage& message,
		std::list<Store*> storeTo,
		const std::string ancestor,
		RunType runType)
{
	LOGI("No ancestor");

	std::string file;
	std::string size;
	std::string changed;
	std::string checksum;
	while(ds.receiveMessage(file) && ds.receiveMessage(changed) && ds.receiveMessage(size)) // && ds.receiveMessage(checksum))
	{
		LOGI("Receiving size = " + size);
		//backupFile(ds, newPath.toString(), StringUtils::fromString<size_t>(size), priority);

		SimpleTee tee;

		std::list<FileAssociations> pointers;

		for(std::list<Store*>::iterator it = storeTo.begin();
				it != storeTo.end();
				++it)
		{
			try
			{
				const Poco::Path originalPath(file);

				Poco::File sourceFile = (*it)->getOutputForCompleteFile(
							originalPath,
							message.getHostIdentification(),
							message.getSourceName(),
							message.getTime());
				Poco::FileOutputStream* fileOutputStream
					= new Poco::FileOutputStream(
							sourceFile.path(),
							std::ios::out | std::ios::trunc);

				SimpleOStreamStream* outputStream
					= new SimpleOStreamStream(*fileOutputStream);

				pointers.push_back(FileAssociations(fileOutputStream, outputStream, sourceFile));
				tee.addOutput(*outputStream);
			}
			catch(Poco::FileException& e)
			{
				LOGE(std::string("Cannot store file because of exception: ") + e.what());
			}
		}
		
		// We must keep receiving the file! If not the protocol gets out of sync
		// and we're screwed.
		const std::streamsize received = StreamUtilities::copyStream(ds, tee, 65536, StringUtils::fromString<size_t>(size));
		if(received != static_cast<std::streamsize>(StringUtils::fromString<size_t>(size)))
		{
			LOGE("Received sizes do not match -- I should do something about this.");
			LOGE("Quitting receival!");
			return;
		}
		LOGI("Bytes received = " + StringUtils::toString(received));

		for(std::list<FileAssociations>::iterator it = pointers.begin();
				it != pointers.end();
				it++)
		{
			it->fileOutputStream->close();
			it->file.setLastModified(
					Poco::Timestamp::fromUtcTime(
						StringUtils::fromString<Poco::Timestamp::UtcTimeVal>(
							changed)*10000000));
			delete it->fileOutputStream;
			delete it->ostreamStream;
		}
		
	}

	// Register this run with every store we saved it to
	for(std::list<Store*>::iterator it = storeTo.begin();
				it != storeTo.end();
				++it)
	{
		(*it)->newCompleteRun(
				message.getHostIdentification(),
				message.getSourceName(),
				message.getTime());
	}

}


}
}
