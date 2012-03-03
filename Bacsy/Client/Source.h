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

#ifndef BACSY_SOURCE_H
#define BACSY_SOURCE_H

#include <string>
#include <set>
#include "Poco/File.h"
#include "Poco/Glob.h"
#include "Poco/Mutex.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Timer.h"
#include "Poco/DirectoryIterator.h"
#include "woodcutter/woodcutter.h"
#include "Bacsy/Client/ISourceConfiguration.h"
#include "Bacsy/Client/PreviousRunRecordFactory.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Common/PocoFile.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;
using namespace Rules;

class Source : public ISourceConfiguration
{
public:
	Source(
			const ISourceConfiguration& config,
			Poco::ThreadPool& threadpool,
			PreviousRunRecordFactory& previousRunRecordFactory);
	~Source();

	void start();

	void run(Poco::Timer& timer);

	std::string getName() const;
	IncludeCollection getIncludes() const;
	ExcludeCollection getExcludes() const;
	unsigned int getPriority() const;
	unsigned int getMinBackups() const;
	unsigned int getMaxBackups() const;
	PreferredOrder getPreferredOrder() const;
	Distribution getDistribution() const;
	TimeTable getTimeTable() const;
	bool getEnabled() const;
	bool getDryPrintRun() const;
	std::string getHostIdentification() const;

private:
	const std::string name;
	const std::vector<std::string> includes;
	const unsigned int priority;
	const unsigned int minBackups;
	const unsigned int maxBackups;
	const ISourceConfiguration::PreferredOrder preferredOrder;
	const ISourceConfiguration::Distribution distribution;
	const ISourceConfiguration::TimeTable timeTable;
	const std::string hostIdentification;
	const bool dryPrintRun;
	const bool enabled;

	std::list<ExclusionRule> exclusionRules;
	std::list<Poco::Timer*> timers;

	Poco::ThreadPool& threadPool;

	Poco::Mutex mutex;

	PreviousRunRecordFactory& previousRunRecordFactory;

	bool isPath(std::string s) const;

	std::list<Poco::Timer*> createTimers();
	void startTimers();

	std::vector<Poco::Net::SocketAddress> findOutWhoToContact();
	void sendCanStore(
			Poco::Net::DatagramSocket& sendFrom,
			Poco::Net::SocketAddress to) const;
	void sendTo(const Poco::Net::SocketAddress& to, const bool saveRunData);

	template<typename FUNCTION>
	void sendAll(
			FUNCTION fun,
			ExclusionRule extraRule,
			PreviousRunRecord* prrData);

	template<typename FUNCTION>
	void backupPath(
			const Poco::File& path,
			FUNCTION& function,
			ExclusionRule extraRule,
			PreviousRunRecord* prrData) const;

	bool isExcluded(
			const ExclusionRule& exclusionRule,
			const Poco::File& path) const;
};

template<typename FUNCTION>
void Source::sendAll(
		FUNCTION fun,
		ExclusionRule extraRule,
		PreviousRunRecord* prrData)
{
	for( std::vector<std::string>::const_iterator it = includes.begin();
			it != includes.end();
			it++)
	{
		backupPath(Poco::File(*it), fun, extraRule, prrData);
	}
}


template<typename FUNCTION>
void Source::backupPath(
		const Poco::File& path,
		FUNCTION& function,
		ExclusionRule extraRule,
		PreviousRunRecord* prrData) const
{
	std::string pathString = path.path();

	// Encapsulate all non-terminal stuff in a try-catch. If it fails that's
	// very unfortunate and an error can be displayed, but life (and the
	// backup) keeps going on. This tends to happen when files are locked in
	// unexpected ways or other strange things are going on. 
	try
	{
		LOGI("Filename = " + pathString);

		if(!path.exists())
		{
			LOGE("Could not backup file " + pathString +
					" because it does not exist.");
			return;
		}

		if(!path.canRead())
		{
			LOGE("Could not backup file " + pathString +
					" because this user does not have read permissions.");
			return;
		}

		if(isExcluded(extraRule, path))
		{
			LOGI("Exclude rule matched.");
			return;
		}

		if(path.isDirectory())
		{
			LOGI("Path is a directory -- expanding.");

			Poco::DirectoryIterator end;
			for(Poco::DirectoryIterator it(path); it != end; ++it)
			{
				backupPath(*it, function, extraRule, prrData);
			}
			return;
		}
	}
	catch(std::exception e)
	{
		LOGE("File " + pathString + " could not be backed up due to error " +
				e.what());
		return;
	}

	LOGI("Path is a file -- not expanding.");
	LOGI("Modified date = " +
			StringUtils::toString<Poco::Timestamp::UtcTimeVal>(
				path.getLastModified().utcTime()/10000000));

	function(path);

	if(prrData != 0)
	{
		prrData->setFileBackedUp(pathString);
	}
}

}
}
#endif
