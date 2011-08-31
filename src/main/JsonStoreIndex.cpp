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
#include "woodcutter/woodcutter.h"
#include "JsonStoreIndex.h"

namespace bacsy
{

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

void JsonStoreIndex::addNewFullRun(const std::string& source, const std::string& directory)
{
	Json::Value insert;
	insert["type"] = "full";
	insert["dir"] = directory;

	root[source].append(insert);

	store();
}

void JsonStoreIndex::addNewDeltaRun(
		const std::string& source,
		const std::string& directory,
		const std::string& builtFromDir)
{
	Json::Value insert;
	insert["type"] = "delta";
	insert["dir"] = directory;
	insert["builtFromDir"] = builtFromDir;

	root[source].append(insert);

	store();
}


std::string JsonStoreIndex::getCorrespondingFullRunForDelta(const std::string& source, const std::string& dir) const
{
	const Json::Value sourceValue = root[source];

	// Find last delta
	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[sourceValue.size() - i - 1];
		if(entry["type"].asString() == "delta" && entry["dir"].asString() == dir)
		{
			return entry["builtFromDir"].asString();
		}
	}
	return "";
}

bool JsonStoreIndex::isDeltaRun(
			const std::string& source,
			const std::string& dir)
{
	const Json::Value sourceValue = root[source];

	// Find last delta
	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[sourceValue.size() - i - 1];
		if(entry["type"].asString() == "delta" && entry["dir"].asString() == dir)
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
std::string JsonStoreIndex::getLastFullRun(const std::string& source) const
{
	const Json::Value sourceValue = root[source];

	for(size_t i = 0; i < sourceValue.size(); i++)
	{
		const Json::Value& entry = sourceValue[sourceValue.size() - i - 1];
		if(entry["type"].asString() == "full")
		{
			return entry["dir"].asString();
		}
	}
	return "";
}

void JsonStoreIndex::store()
{
	const std::string& newContents = writer.write(root);
	
	std::ofstream out(path.c_str());
	out << newContents;
	out.close();
}

}
