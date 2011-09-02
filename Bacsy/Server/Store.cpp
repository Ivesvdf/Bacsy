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
#include "Poco/File.h"
#include "Poco/ScopedLock.h"
#include "Poco/Ascii.h"
#include "Bacsy/Common/Functional.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Server/Store.h"

namespace Bacsy
{

Store::Store(const IStoreConfiguration& configuration):
	storeName(configuration.getName()),
	location(StringUtils::rstrip(configuration.getLocation(), "/\\") + "/"),
	alwaysPresent(configuration.getAlwaysPresent()),
	minPriorityForStoring(configuration.getMinPriorityForStoring()),
	baseLocation(location)

{
	// Create the main directory for this store at start if it doesn't exist
	if(!baseLocation.exists() && alwaysPresent)
		baseLocation.createDirectories();

	storeIndex = new JsonStoreIndex(location + "index.json");
}

Store::~Store()
{
	delete storeIndex;
}

std::string Store::getAncestorForNewRun(const std::string& ancestor)
{
	return "";
}


void Store::newCompleteRun(
			const std::string& host,
			const std::string& source,
			const std::string& runID)
{
	Poco::ScopedLock<Poco::FastMutex> lock(storeIndexMutex);

	storeIndex->addNewFullRun(source, getRunDirectory(host, source, runID));
}

std::string Store::getRunDirectory(
			const std::string& host,
			const std::string& source,
			const std::string& runID)
{
	return "[" + host + "][" + source + "] " + runID;
}

Poco::File Store::getOutputForCompleteFile(
		const Poco::Path& originalPath,
		const std::string& host, 
		const std::string& source, 
		const std::string& runID)
{
	Poco::Path newPath(location);

	newPath.pushDirectory(getRunDirectory(host, source, runID));

	// Keep only alphabetic characters in the nodeID
	std::string nodeIdentification(originalPath.getNode());
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

	return Poco::File(newPath);
}

bool Store::readyForStoring() const
{
	// If the location was AlwaysPresent but did not exist,
	// the necessary directories would have been created in the Store ctor. 
	return baseLocation.exists(); 
}

std::string Store::toString() const
{
	return storeName 
		+ " @ " 
		+ location 
		+ "\tMSP=" 
		+ StringUtils::toString(minPriorityForStoring) 
		+ ",\t AP=" + StringUtils::toString(alwaysPresent);
}

}
