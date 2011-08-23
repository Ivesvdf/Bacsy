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

#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include <set>
#include "Poco/File.h"
#include "Poco/Glob.h"
#include "Poco/Mutex.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Timer.h"
#include "CascadingFileConfiguration.h"

class Source
{
private:
	const std::string name;
	const std::vector<std::string> includes;
	const std::vector<std::string> excludes;
	const unsigned int priority;
	const unsigned int minBackups;
	const unsigned int maxBackups;
	const std::string preferredOrder;
	const std::string distribution;
	const std::string timerString;
	const std::string hostIdentification;

	std::list<ExclusionRule> exclusionRules;
	std::list<Poco::Timer*> timers;

	Poco::Mutex mutex;

	bool isPath(std::string s) const;

	std::list<Poco::Timer*> createTimers(const std::string& timerString);
	void startTimers();

	std::vector<Poco::Net::SocketAddress> findOutWhoToContact();
	void sendCanStore(Poco::Net::DatagramSocket& sendFrom, Poco::Net::SocketAddress to) const;
	void sendTo(const Poco::Net::SocketAddress& to);

public:
	Source(std::string sectionName, const CascadingFileConfiguration& config);
	~Source();

	void start();

	void run(Poco::Timer& timer);


private:
	template<typename FUNCTION>
	void backupPath(const Poco::File& path, FUNCTION& function) const
	{
		std::string pathString = path.path();
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

		for(std::list<ExclusionRule>::const_iterator it = exclusionRules.begin();
				it != exclusionRules.end();
				++it)
		{
			if(it->match(path))
			{
				LOGI("Exclude rule matched.");
				return;
			}
		}

		if(path.isDirectory())
		{
			LOGI("Path is a directory -- expanding.");
			std::vector<Poco::File> directoryContents;
			path.list(directoryContents);

			for(std::vector<Poco::File>::const_iterator it = directoryContents.begin();
					it != directoryContents.end();
					it++)
			{
				backupPath(*it, function);
			}
		}
		else // Is file
		{
			LOGI("Path is a file -- not expanding.");
			LOGI("Modified date = " + StringUtils::toString<Poco::Timestamp::UtcTimeVal>(path.getLastModified().utcTime()/10000000));
			function(path);
		}


	}
};

#endif
