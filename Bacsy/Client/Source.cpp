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

#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Environment.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "woodcutter/woodcutter.h"
#include "json/json.h"
#include "Bacsy/Client/Source.h"
#include "Bacsy/Client/TimerStringParser.h"
#include "Bacsy/Common/Info.h"
#include "Bacsy/Common/DatagramHelper.h"
#include "Bacsy/Common/JsonHelper.h"
#include "Bacsy/Common/IFile.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;
using namespace Rules;

bool Source::isPath(std::string s) const
{
	return s.find_first_of("/\\") != std::string::npos;
}

Source::Source(const ISourceConfiguration& config):
	name(config.getName()),
	includes(config.getIncludes()),
	priority(config.getPriority()),
	minBackups(config.getMinBackups()),
	maxBackups(config.getMaxBackups()),
	preferredOrder(config.getPreferredOrder()),
	distribution(config.getDistribution()),
	timeTable(config.getTimeTable()),
	hostIdentification(config.getHostIdentification()),
	dryPrintRun(config.getDryPrintRun()),
	enabled(config.getEnabled()),
	exclusionRules(config.getExcludes()),
	timers(createTimers())
{
}

void Source::startTimers()
{
	for(std::list<Poco::Timer*>::iterator it = timers.begin();
			it != timers.end();
			it++)
	{
		LOGI("Starting timer");
		// Create a new callback that calls the classes run method
		Poco::TimerCallback<Source> callback(*this, &Source::run);
		// And run it on the the item that was added last (the one we just
		// added)
		(*it)->start(callback);
	}
}

std::list<Poco::Timer*> Source::createTimers()
{
	std::list<Poco::Timer*> theTimers;
	for(ISourceConfiguration::TimeTable::const_iterator it = timeTable.begin();
			it != timeTable.end();
			it++)
	{
		// Execute all timers that normally fire immediately after 1 second
		const long delayInMilliSeconds = (it->delay.totalMilliseconds() == 0) ? 1000 : it->delay.totalMilliseconds();
		Poco::Timer* const timer = new Poco::Timer(delayInMilliSeconds, it->repeat.totalMilliseconds());
		LOGI(std::string("Creating timer with delay of ")
				+ StringUtils::toString(delayInMilliSeconds) 
				+ std::string(" and repeat rate of ")
				+ StringUtils::toString(it->repeat.totalMilliseconds()));
		theTimers.push_back(timer);
	}
	return theTimers;
}

Source::~Source()
{
	for(std::list<Poco::Timer*>::iterator it = timers.begin();
			it != timers.end();
			it++)
	{
		(*it)->stop();
		delete (*it);
	}
}

void Source::start()
{
	startTimers();
}

class PrintFileSender
{
public:
	PrintFileSender()
	{}

	inline void operator()(const Poco::File& file)
	{
		std::cout << file.path() << std::endl;
	}
};


void Source::run(Poco::Timer& timer)
{
	LOGI("Starting source " 
			+ name 
			+ " because of Timer(" 
			+ StringUtils::toString(timer.getStartInterval()) 
			+ "," 
			+ StringUtils::toString(timer.getPeriodicInterval()) 
			+ ")");

	if(maxBackups == 0)
	{
		LOGI("Not executing source " + name + " after all, because maxBackups equals 0.");
		return;
	}

	if(!enabled)
	{
		LOGI("Not executing source " + name + ", because it's disabled.");
		return;
	}

	if(!mutex.tryLock())
	{
		LOGW("Source is already running -- not executing again.");
		return;
	}

	if(dryPrintRun)
	{
		LOGI("Executing Dry Print Run");
		PrintFileSender sender;
		sendAll(sender);
	}
	else
	{
		std::vector<Poco::Net::SocketAddress> peopleToContact = findOutWhoToContact();

		for(std::vector<Poco::Net::SocketAddress>::const_iterator it = peopleToContact.begin();
				it != peopleToContact.end();
				it++)
		{
			Poco::Net::SocketAddress contact(it->host(), BACSYSERVERPORT);
			LOGI("Contacting " + contact.toString());

			sendTo(contact);
		}
	}

	mutex.unlock();

	LOGI("Source " + name + " is finished.");
}


std::string Source::getName() const
{
	return name;
}

std::vector<std::string> Source::getIncludes() const
{
	return includes;
}

std::list<ExclusionRule>  Source::getExcludes() const
{
	return exclusionRules;
}

unsigned int Source::getPriority() const
{
	return priority;
}

unsigned int Source::getMinBackups() const
{
	return minBackups;
}

unsigned int Source::getMaxBackups() const
{
	return maxBackups;
}

ISourceConfiguration::PreferredOrder Source::getPreferredOrder() const
{
	return preferredOrder;
}

ISourceConfiguration::Distribution Source::getDistribution() const
{
	return distribution;
}

ISourceConfiguration::TimeTable Source::getTimeTable() const
{
	return timeTable;
}

bool Source::getEnabled() const
{
	return enabled;
}

bool Source::getDryPrintRun() const
{
	return dryPrintRun;
}

std::string Source::getHostIdentification() const
{
	return hostIdentification;
}

class FileSender
{
public:
	FileSender(Poco::Net::DialogSocket& socket):
		socket(socket)
	{}

	inline void operator()(const Poco::File& file)
	{
		LOGI("Should be sending file...");
		LOGI("Size of file = " + StringUtils::toString(file.getSize()));

		Poco::FileInputStream input(file.path());
		Poco::Net::SocketStream output(socket);

		socket.sendMessage(file.path());
		socket.sendMessage(StringUtils::toString(file.getLastModified().utcTime()/10000000));
		socket.sendMessage(StringUtils::toString(file.getSize()));

		Poco::StreamCopier::copyStream(input, output, 65536);
	}

private:
	Poco::Net::DialogSocket& socket;
};

void Source::sendTo(const Poco::Net::SocketAddress& who)
{
	Poco::Net::DialogSocket socket(who);
	socket.sendMessage(bacsyProtocolString);
	Json::Value root;
	root["type"] = "store";
	root["host"] = hostIdentification;
	root["source"] = name;
	root["priority"] = priority;
	root["runID"] = Poco::DateTimeFormatter::format(
			Poco::Timestamp(),
			"%Y-%m-%dT%H.%M.%S%z");
	// TODO: Change this to an actual limit...
	root["maxStoreTimes"] = maxBackups;

	socket.sendMessage(JsonHelper::write(root));

	FileSender sender(socket);
	sendAll(sender);
}

void Source::sendCanStore(Poco::Net::DatagramSocket& sendFrom, Poco::Net::SocketAddress to) const
{
	Json::Value root;
	root["type"] = "canStore";
	root["source"] = name;
	root["priority"] = priority;

	const std::string msg = DatagramHelper::toMessage(root);

	LOGI("Sending canStore message");
	sendFrom.sendTo(msg.c_str(), msg.size(), to);
}

class CanStoreResponseAccepter
{
	public:
		CanStoreResponseAccepter(const std::string sourceName):
			sourceName(sourceName)
		{}

		void operator()(Poco::Net::SocketAddress who, const std::string& what)
		{
			LOGI("Received Multicast message.");

			std::vector<std::string> parts = StringUtils::split(what, "\n");

			if(parts.size() != 2 || parts[0] != bacsyProtocolString)
			{
				LOGW( "Received packet with incorrect protocol string or invalid contents:"
						+ what);
				return;
			}

			try
			{
				const Json::Value root = JsonHelper::read(parts[1]);
				if(root["type"] != "readyToStore" || root["source"] != sourceName)
					return;
			}
			catch(const std::runtime_error& e)
			{
				LOGW(e.what());
				return;
			}	

			LOGI(std::string("Received valid MC message -- adding ") + who.toString());
			peopleToContact.push_back(who);

		}

		std::vector<Poco::Net::SocketAddress> getPeopleToContact() const 
		{ return peopleToContact; }


	private:
		std::vector<Poco::Net::SocketAddress> peopleToContact;
		std::string sourceName;
};

std::vector<Poco::Net::SocketAddress> Source::findOutWhoToContact()
{
	Poco::Net::SocketAddress address(MULTICASTGROUP, MULTICASTPORT);

	Poco::Net::MulticastSocket mcSocket(
			Poco::Net::SocketAddress(
				Poco::Net::IPAddress(), address.port()),
			true // reuse address
			);

	// to receive any data you must join
	LOGI("Joining multicast group " + MULTICASTGROUP);
	mcSocket.joinGroup(address.host());
	LOGI("Joined multicast group.");
	
	sendCanStore(mcSocket, address);
	
	CanStoreResponseAccepter accepter(name);

	DatagramHelper::receiveMessages<512>(mcSocket, 1000, accepter);

	return accepter.getPeopleToContact();
}

bool Source::isExcluded(const Poco::File& path) const
{
	for(std::list<ExclusionRule>::const_iterator it = exclusionRules.begin();
			it != exclusionRules.end();
			++it)
	{
		if(it->match(PocoFile(path)))
		{
			return true;
		}
	}
	
	return false;
}

}
}
