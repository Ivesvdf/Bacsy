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
#include "Poco/DirectoryIterator.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Environment.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timestamp.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "Bacsy/Common/Functional.h"
#include "Bacsy/Common/FileUtils.h"
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

		Bacsy::Messages::StoreResponseMessage responseMessage(
				spec.runType,
				spec.time);

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
			LOGE("Neither Files nor Fullfiles run -- not implemented.");
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
		RunType& runType)
{
	LOGI("RunType = " + runType.toString());

	std::string file;
	std::string size;
	std::string changed;
	std::string checksum;

	bool wroteSomething = false;

	while(true)
	{
		const bool b1 = ds.receiveMessage(file);

		// Test if a newline was received, if so we need to exit the loop
		// because we'll be getting e.g. EOF later.
		if(b1 && file.empty())
		{
			break;
		}

		const bool b2 = ds.receiveMessage(changed);
		const bool b3 = ds.receiveMessage(size);


		if(!b1 || file.empty())
		{
			LOGE("Problem receiving filename");
			break;
		}
		if(!b2 || changed.empty())
		{
			LOGE("Problem receiving changed date");
			break;
		}
		if(!b3 || size.empty())
		{
			LOGE("Problem receiving file metadata.");
			break;
		}

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
							runType,
							originalPath,
							message.getHostIdentification(),
							message.getSourceName(),
							message.getTime());
				LOGI("Preparing to write to " + sourceFile.path());

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

		wroteSomething = true;

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

	std::string possibleEOF;
	ds.receiveMessage(possibleEOF);

	if(possibleEOF == "EOF")
	{
		LOGI("EOF Received.");

		// Register this run with every store we saved it to
		for(std::list<Store*>::iterator it = storeTo.begin();
					it != storeTo.end();
					++it)
		{

			if(runType == RunType::full)
			{
				LOGI("Registering full run for store " + (*it)->toString());
				(*it)->newCompleteRun(
						message.getHostIdentification(),
						message.getSourceName(),
						message.getTime());
			}
			else if(runType == RunType::fullfiles)
			{
				LOGI("Swapping files");
				Poco::Path old((*it)->getLocation());
				old.makeDirectory();
				old.pushDirectory(ancestor);
				Poco::File walkDir = Poco::File((*it)->getBaseOutputDirectoryForCompleteFile(
							runType, 
							message.getHostIdentification(),
							message.getSourceName(),
							message.getTime()));
				Poco::File oldDir = Poco::File(old);

				if(wroteSomething)
				{
					swapContents(walkDir, oldDir);
				}
				else
				{
					walkDir.createDirectories();
				}

				Poco::Path latestVersion = 
					(*it)->getBaseOutputDirectoryForCompleteFile(
							RunType::full, // need official path
							message.getHostIdentification(),
							message.getSourceName(),
							message.getTime());
				LOGI("Renaming directory " + oldDir.path() + " to " +
						latestVersion.toString());
				std::string oldOldDirPath = oldDir.path();
				oldDir.renameTo(latestVersion.toString());

				LOGI("Renaming " + walkDir.path() + " to " + oldOldDirPath);
				walkDir.renameTo(oldOldDirPath);

				LOGI("Registering fullfiles run for store " + (*it)->toString());
				(*it)->newFullfilesRun(
						message.getHostIdentification(),
						message.getSourceName(),
						ancestor,
						message.getTime());
			}

		}
	}
	else
	{
		LOGE("No EOF received. Not writing to store indexes.");
	}

}

void BacsyConnection::swapContents(Poco::File walkPath, Poco::File otherPath)
{
	Poco::DirectoryIterator end;
	for(Poco::DirectoryIterator it(walkPath); it != end; ++it)
	{
		Poco::Path newOtherPath(otherPath.path());
		newOtherPath.append(Poco::Path(it->path())
				.directory(Poco::Path(it->path()).depth()));

		swap(*it, Poco::File(newOtherPath));
	}
}

void BacsyConnection::swap(Poco::File walkPath, Poco::File otherPath)
{
	LOGI("Swapping " + walkPath.path() + " and " + otherPath.path());


	if(walkPath.isDirectory())
	{
		LOGI("Path is a directory -- expanding.");

		Poco::DirectoryIterator end;
		for(Poco::DirectoryIterator it(walkPath); it != end; ++it)
		{
			Poco::Path newOtherPath(otherPath.path());
			newOtherPath.append(Poco::Path(it->path())
					.directory(Poco::Path(it->path()).depth()));

			swap(*it, Poco::File(newOtherPath));
		}
	}
	else if(!otherPath.exists())
	{
		Poco::Path targetDir(otherPath.path());
		targetDir.makeFile();
		targetDir.setFileName("");
		targetDir.makeDirectory();
		LOGI("Moving file " + walkPath.path() + " to " + targetDir.toString()
				+ " because " + otherPath.path() + " does not exist.");
		walkPath.moveTo(targetDir.toString());
		return;
	}
	else // Is file that exists
	{
		FileUtils::swapContents(walkPath, otherPath);
		LOGI("Files swapped");
	}
}


}
}
