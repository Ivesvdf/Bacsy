/*
 * Copyright (C) 2011  Ives van der Flaas
 * Copyright (C) 2011  Nathan Samson
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

#ifndef STORE_H
#define STORE_H

#include <string>
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include "Poco/File.h"
#include "Bacsy/Server/JsonStoreIndex.h"
#include "Bacsy/Server/IStoreConfiguration.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class Store : public IStoreConfiguration
{
public:
	Store(const IStoreConfiguration& configuration);
	~Store();

	std::string getAncestorDirectoryForNewRun(const std::string& ancestor);

	Poco::File getOutputForCompleteFile(
			const Poco::Path& originalPath,
			const std::string& host,
			const std::string& source,
			const Poco::Timestamp& time);

	void newCompleteRun(
			const std::string& host,
			const std::string& source,
			const Poco::Timestamp& time);

	void newFullfilesRun(
			const std::string& host,
			const std::string& source,
			const std::string& ancestorDirectory,
			const Poco::Timestamp& time);

	bool readyForStoring() const;

	std::string toString() const;

	std::string getName() const;
	unsigned int getMinPriorityForStoring() const;
	std::string getLocation() const;
	bool getAlwaysPresent() const;
	bool getEnabled() const;
private:
	std::string getRunDirectory(
			const std::string& host,
			const std::string& source,
			const Poco::Timestamp& time);

	const std::string storeName;

	const std::string location;
	const bool alwaysPresent;
	const unsigned int minPriorityForStoring;
	const bool enabled;

	Poco::File baseLocation;

	JsonStoreIndex* storeIndex;
	Poco::FastMutex storeIndexMutex;
};

}
}
#endif
