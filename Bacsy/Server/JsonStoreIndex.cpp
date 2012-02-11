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

#include <fstream>
#include <stdexcept>
#include "woodcutter/woodcutter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Bacsy/Common/RunType.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Server/JsonStoreIndex.h"

namespace Bacsy
{
namespace Server
{

using Bacsy::Common::RunType;
using Bacsy::Common::StringUtils;

JsonStoreIndex::JsonStoreIndex(const std::string inputPath):
	path(inputPath)
{
	read();
}

void JsonStoreIndex::read()
{
	std::ifstream in(path.c_str());

	if(!in)
	{
		LOGI("StoreIndex file does not exist -- will create one later.");
		return;
	}

	if (!reader.parse(in, root))
	{
		LOGE("Could not read from store index file " + path);
		return;
	}	
}

void JsonStoreIndex::addNewRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& type,
		const std::string& dir,
		const Poco::Timestamp& time,
		const std::string& builtFromDir)
{
	Json::Value insert;
	insert["type"] = type;
	insert["dir"] = dir;
	if(builtFromDir != "")
	{
		insert["builtFromDir"] = builtFromDir;
	}


	insert["time"] = Json::Int64(time.utcTime());

	root[hostIdentification][source].append(insert);

	store();
}

void JsonStoreIndex::deleteDirectories(
		const std::string& hostIdentification,
		const std::string& source,
		const std::set<std::string>& directories)
{
	Json::Value sourceValue = root[hostIdentification][source];

	// No remove for array elements? I guess I'll do it the hard way
	Json::Value copy = sourceValue;
	sourceValue.clear();


	for(Json::Value::ArrayIndex i = 0; i < copy.size(); i++)
	{
		const Json::Value& entry = copy[i];

		if(directories.count(entry["dir"].asString()) == 0)
		{
			sourceValue.append(entry);
		}
	}

	// Now erase all references to the deleted runs
	for(Json::Value::ArrayIndex i = 0; i < sourceValue.size(); i++)
	{
		Json::Value& entry = sourceValue[i];

		if(directories.count(entry["builtFromDir"].asString()) != 0)
		{
			entry.removeMember("builtFromDir");
		}
	}
	

	root[hostIdentification][source] = sourceValue;
	store();
}

std::vector<std::string> JsonStoreIndex::getRunsOlderThan(
	const std::string& hostIdentification,
	const std::string& source,
	const Poco::Timestamp& time)
{
	const Json::Value sourceValue = root[hostIdentification][source];
	std::vector<std::string> rv;

	// Initialise at 1 to skip the last backup. Never delete the last backup.
	for(size_t i = 1; i < sourceValue.size(); i++)
	{
		const Json::Value& entry =
			sourceValue[(Json::Value::ArrayIndex)
				(sourceValue.size() - i - 1)];
		if(entry["time"].asInt64() < time.utcTime())
		{
			rv.push_back(entry["dir"].asString());
		}
	}
	return rv;

}


void JsonStoreIndex::addNewFullFilesRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& directory,
		const std::string& builtFromDir,
		const Poco::Timestamp& time)
{
	addNewRun(
			hostIdentification,
			source,
			RunType::fullfiles.toString(),
			directory,
			time,
			builtFromDir);
}

void JsonStoreIndex::addNewFullRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& directory,
		const Poco::Timestamp& time)
{
	addNewRun(
			hostIdentification,
			source,
			RunType::full.toString(),
			directory,
			time);
}

void JsonStoreIndex::addNewDeltaRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& directory,
		const std::string& builtFromDir,
		const Poco::Timestamp& time)
{
	addNewRun(
			hostIdentification,
			source,
			RunType::delta.toString(),
			directory,
			time,
			builtFromDir);
}


std::string JsonStoreIndex::getCorrespondingFullRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& dir) const
{
	const Json::Value sourceValue = root[hostIdentification][source];

	// Find last delta
	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		if(entry["dir"].asString() == dir)
		{
			return entry["builtFromDir"].asString();
		}
	}
	return "";
}

bool JsonStoreIndex::isDeltaRun(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& dir)
{
	const Json::Value sourceValue = root[hostIdentification][source];

	// Find last delta
	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		if(entry["type"].asString() == 	RunType::delta.toString()
				&& entry["dir"].asString() == dir)
		{
			return true;
		}
	}
	return false;
}

/**
 * @return the directory in which the last full run can be found if there
 * ever was a full run for this source. If not the empty string will be
 * returned.
 */
std::string JsonStoreIndex::getLastFullRun(
		const std::string& hostIdentification,
		const std::string& source) const
{
	const Json::Value sourceValue = root[hostIdentification][source];

	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		if(entry["type"].asString() == 	RunType::full.toString())
		{
			return entry["dir"].asString();
		}
	}
	return "";
}

unsigned int JsonStoreIndex::getNrOfNonFullRunsAfterLastFull(
		const std::string& hostIdentification,
		const std::string& source) const
{
	const Json::Value sourceValue = root[hostIdentification][source];
	int nonfulls = 0;

	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		if(entry["type"].asString() == 	RunType::full.toString())
		{
			return nonfulls;
		}
		else
		{
			nonfulls++;
		}
	}

	return nonfulls;
}

std::string JsonStoreIndex::getLastRun(
		const std::string& hostIdentification,
		const std::string& source) const
{
	const Json::Value sourceValue = root[hostIdentification][source];

	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		return entry["dir"].asString();
	}
	return "";
}

Poco::Timestamp JsonStoreIndex::getCorrespondingTime(
		const std::string& hostIdentification,
		const std::string& source,
		const std::string& directory) const
{
	const Json::Value sourceValue = root[hostIdentification][source];

	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[(Json::Value::ArrayIndex)(sourceValue.size() - i - 1)];
		if(entry["dir"].asString() == directory)
		{
			return Poco::Timestamp::fromUtcTime(entry["time"].asInt64());
		}
	}

	throw std::runtime_error("No such directory for host & source.");
}

Poco::Timestamp JsonStoreIndex::getLastRunTime(
		const std::string& hostIdentification,
		const std::string& source) const
{
	const Json::Value sourceValue = root[hostIdentification][source];
	return Poco::Timestamp::fromUtcTime(
			sourceValue[sourceValue.size()-1]["time"].asInt64());
}

void JsonStoreIndex::store()
{
	const std::string& newContents = writer.write(root);
	
	std::ofstream out(path.c_str());
	out << newContents;
	out.close();
}

}
}
