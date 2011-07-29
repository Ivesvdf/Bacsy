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

#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <set>
#include "Poco/File.h"
#include "Poco/Mutex.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Timer.h"
#include "cascadingFileConfiguration.h"

class Target
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

	std::vector<std::string> fileGlobExcludes;
	std::vector<std::string> pathGlobExcludes;
	std::set<std::string> pathExcludes;
	std::list<Poco::Timer*> timers;

	Poco::Mutex mutex;

	void backupPath(const Poco::File& path) const;
	bool isPath(std::string s) const;

	std::list<Poco::Timer*> createTimers(const std::string& timerString);
	void startTimers();

	std::vector<Poco::Net::SocketAddress> findOutWhoToContact();
	void sendCanStore(Poco::Net::DatagramSocket& sendFrom, Poco::Net::SocketAddress to) const;
	void sendTo(const Poco::Net::SocketAddress& to);

public:
	Target(std::string sectionName, const CascadingFileConfiguration& config);
	~Target();

	void start();

	void run(Poco::Timer& timer);
};

#endif
