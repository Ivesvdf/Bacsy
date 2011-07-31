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
#include "target.h"
#include "timerStringParser.h"
#include "info.h"
#include "datagramHelper.h"
#include "jsonHelper.h"

bool Target::isPath(std::string s) const
{
	return s.find_first_of("/\\") != std::string::npos;
}

Target::Target(std::string section, const CascadingFileConfiguration& config):
	name(section),
	includes(config.getIncludes(section)),
	excludes(config.getExcludes(section)),
	priority(config.getPriority(section)),
	minBackups(config.getMinBackups(section)),
	maxBackups(config.getMaxBackups(section)),
	preferredOrder(config.getPreferredOrder(section)),
	distribution(config.getDistribution(section)),
	hostIdentification(config.getHostIdentification(section)),
	timers(createTimers(config.getTimerString(section)))
{
	// Handle excludes
	for(std::vector<string>::const_iterator it = excludes.begin();
			it != excludes.end();
			it++)
	{
		const std::string& subject = *it;

		if(subject.find('*') != std::string::npos || subject.find('?') != std::string::npos)
		{
			// Probable glob found ... (no problem if it turns
			// out this wasn't a glob; except for the performance hit
			// obviously)
			if(isPath(subject))
			{
				pathGlobExcludes.push_back(subject);
			}
			else
			{
				// If the glob is not a path glob, it's a file glob
				fileGlobExcludes.push_back(subject);
			}
		}
		else
		{
			// No signs this is a glob, interpret as path
			pathExcludes.insert(StringUtils::rstrip(subject, "/\\"));
		}
	}
}

void Target::startTimers()
{
	for(std::list<Poco::Timer*>::iterator it = timers.begin();
			it != timers.end();
			it++)
	{
		LOGI("Starting timer");
		// Create a new callback that calls the classes run method
		Poco::TimerCallback<Target> callback(*this, &Target::run);
		// And run it on the the item that was added last (the one we just
		// added)
		(*it)->start(callback);
	}
}

std::list<Poco::Timer*> Target::createTimers(const std::string& timerString)
{
	std::list<Poco::Timer*> theTimers;
	TimerStringParser parser;
	std::list<TimeSchedule> schedules = parser.parse(Poco::LocalDateTime(), timerString);
	for(std::list<TimeSchedule>::const_iterator it = schedules.begin();
			it != schedules.end();
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

Target::~Target()
{
	for(std::list<Poco::Timer*>::iterator it = timers.begin();
			it != timers.end();
			it++)
	{
		(*it)->stop();
		delete (*it);
	}
}

void Target::start()
{
	startTimers();
}

void Target::run(Poco::Timer& timer)
{
	LOGI("Starting target " 
			+ name 
			+ " because of Timer(" 
			+ StringUtils::toString(timer.getStartInterval()) 
			+ "," 
			+ StringUtils::toString(timer.getPeriodicInterval()) 
			+ ")");

	if(!mutex.tryLock())
	{
		LOGW("Target is already running -- not executing again.");
		return;
	}

	std::vector<Poco::Net::SocketAddress> peopleToContact = findOutWhoToContact();

	for(std::vector<Poco::Net::SocketAddress>::const_iterator it = peopleToContact.begin();
			it != peopleToContact.end();
			it++)
	{
		Poco::Net::SocketAddress contact(it->host(), BACSYSERVERPORT);
		LOGI("Contacting " + contact.toString());

		sendTo(contact);
	}

	mutex.unlock();

	LOGI("Target " + name + " is finished.");
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
		socket.sendMessage(StringUtils::toString(file.getSize()));

		Poco::StreamCopier::copyStream(input, output, 65536);
	}

private:
	Poco::Net::DialogSocket& socket;
};

void Target::sendTo(const Poco::Net::SocketAddress& who)
{
	Poco::Net::DialogSocket socket(who);
	socket.sendMessage(bacsyProtocolString);
	Json::Value root;
	root["type"] = "store";
	root["host"] = hostIdentification;
	root["target"] = name;
	root["priority"] = priority;
	root["runID"] = Poco::DateTimeFormatter::format(
			Poco::Timestamp(),
			Poco::DateTimeFormat::ISO8601_FORMAT);

	socket.sendMessage(JsonHelper::write(root));

	FileSender sender(socket);
	for( std::vector<std::string>::const_iterator it = includes.begin();
			it != includes.end();
			it++)
	{
		backupPath(Poco::File(*it), sender);
	}
}

void Target::sendCanStore(Poco::Net::DatagramSocket& sendFrom, Poco::Net::SocketAddress to) const
{
	Json::Value root;
	root["type"] = "canStore";
	root["target"] = name;
	root["priority"] = priority;

	const std::string msg = DatagramHelper::toMessage(root);

	LOGI("Sending canStore message");
	sendFrom.sendTo(msg.c_str(), msg.size(), to);
}

class CanStoreResponseAccepter
{
	public:
		CanStoreResponseAccepter(const std::string targetName):
			targetName(targetName)
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
				if(root["type"] != "readyToStore" || root["target"] != targetName)
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
		std::string targetName;
};

std::vector<Poco::Net::SocketAddress> Target::findOutWhoToContact()
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

