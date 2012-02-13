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
#include "Poco/Timestamp.h"
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
#include "Bacsy/Messages/StoreMessage.h"
#include "Bacsy/Messages/CanStoreMessage.h"
#include "Bacsy/Messages/ReadyToStoreMessage.h"
#include "Bacsy/Messages/StoreResponseMessage.h"
#include "Bacsy/Rules/ModifiedDateExclusionSubRule.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;
using namespace Rules;
using Bacsy::Messages::StoreResponseMessage;
using Bacsy::Messages::ReadyToStoreMessage;

bool Source::isPath(std::string s) const
{
	return s.find_first_of("/\\") != std::string::npos;
}

Source::Source(const ISourceConfiguration& config, Poco::ThreadPool& threadPool):
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
	timers(createTimers()),
	threadPool(threadPool)
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
		(*it)->start(callback, threadPool);
	}
}

std::list<Poco::Timer*> Source::createTimers()
{
	std::list<Poco::Timer*> theTimers;
	for(ISourceConfiguration::TimeTable::const_iterator it = timeTable.begin();
			it != timeTable.end();
			it++)
	{
		const long delayInMilliSeconds = it->delay.totalMilliseconds();
		Poco::Timer* const timer = new Poco::Timer(
				delayInMilliSeconds,
				it->repeat.totalMilliseconds());
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
	try
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
			LOGI("Not executing source " 
					+ name 
					+ " after all, because maxBackups equals 0.");
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
			ExclusionRule rule;
			sendAll(sender, rule);
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
	catch(Poco::Exception& e)
	{
		LOGE(std::string("Terminal Poco exception: ") + e.displayText());
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


std::string Source::getName() const
{
	return name;
}

ISourceConfiguration::IncludeCollection Source::getIncludes() const
{
	return includes;
}

ISourceConfiguration::ExcludeCollection Source::getExcludes() const
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

		socket.sendMessage(Poco::Path::transcode(file.path()));
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

	Bacsy::Messages::StoreMessage(
		hostIdentification,
		name,
		priority,
		maxBackups).send(socket);

	StoreResponseMessage response = StoreResponseMessage::receive(socket);

	if(response.getRunType() == RunType::full)
	{
		LOGI("Sending a full run");
		FileSender sender(socket);
		ExclusionRule rule; // use empty extra rule
		sendAll(sender, rule);
		socket.sendMessage("");
		socket.sendMessage("EOF");
	}
	else if(response.getRunType() == RunType::fullfiles)
	{
		LOGI("Sending a full files run.");
		FileSender sender(socket);
		ExclusionRule rule;
		rule.addSubRule(
				new ModifiedDateExclusionSubRule(
					response.getTimestamp(),
					ModifiedDateExclusionSubRule::OLDER_THAN,
					false));

		sendAll(sender, rule);
		socket.sendMessage("");
		socket.sendMessage("EOF");
	}
	else
	{
		LOGE("Unsupported run type.");
	}
}

void Source::sendCanStore(
		Poco::Net::DatagramSocket& sendFrom,
		Poco::Net::SocketAddress to) const
{
	LOGI("Sending canStore message");
	Bacsy::Messages::CanStoreMessage(name, priority).send(sendFrom, to);
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
				LOGW("Received packet with incorrect protocol "
						"string or invalid contents:" + what);
				return;
			}

			try
			{
				const Json::Value root = JsonHelper::read(parts[1]);
				LOGI(std::string("Multicast message is of type ") +
						root["type"].asString());
				if(root["type"] == "readyToStore")
				{
					if(ReadyToStoreMessage(root).getSource() != sourceName)
					{
						return;
					}
				}
				else
				{
					return;
				}
			}
			catch(const std::runtime_error& e)
			{
				LOGW(e.what());
				return;
			}

			LOGI(std::string("Received valid MC message -- adding ")
					+ who.toString());
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

	Poco::Net::SocketAddress mcSocketAddress(
			Poco::Net::IPAddress(), address.port());
	Poco::Net::MulticastSocket mcSocket(mcSocketAddress,
			true // reuse address
			);

	// to receive any data you must join
	LOGI("Joining multicast group " + MULTICASTGROUP);
	mcSocket.joinGroup(address.host());
	LOGI("Joined multicast group.");
	

	Poco::Net::SocketAddress responseAcceptorAddress(Poco::Net::IPAddress(),
			MULTICASTRESPONSEACCEPTORPORT);
	Poco::Net::DatagramSocket responseSocket(responseAcceptorAddress, true);

	LOGI("Sending canStore");
	sendCanStore(mcSocket, address);
	
	CanStoreResponseAccepter accepter(name);

	LOGI("Listening on " + responseAcceptorAddress.toString());
	DatagramHelper::receiveMessages<512>(responseSocket, 2000, accepter);

	return accepter.getPeopleToContact();
}

bool Source::isExcluded(
		const ExclusionRule& exclusionRule,
		const Poco::File& path) const
{
	if(exclusionRule.match(PocoFile(path)))
	{
		LOGI("Extra rule matched:" + path.path() + " was matched because of"
				" rule " + exclusionRule.toString());
		return true;
	}

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
