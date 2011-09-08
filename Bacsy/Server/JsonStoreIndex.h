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

#ifndef JSON_STORE_INDEX
#define JSON_STORE_INDEX

#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
#include "json/json.h"

namespace Bacsy
{
namespace Server
{

class JsonStoreIndex
{
public:
	JsonStoreIndex(const std::string path);

	void addNewFullRun(
			const std::string& source,
			const std::string& directory,
			const Poco::Timestamp& time);

	void addNewFullFilesRun(
			const std::string& source,
			const std::string& directory,
			const std::string& builtFromDir,
			const Poco::Timestamp& time);

	void addNewDeltaRun(
			const std::string& source,
			const std::string& directory,
			const std::string& builtFromDir,
			const Poco::Timestamp& time);

	std::string getCorrespondingFullRun(
			const std::string& source,
			const std::string& dir) const;

	std::string getLastFullRun(const std::string& source) const;

	bool isDeltaRun(
			const std::string& source,
			const std::string& dir);
private:
	void store();
	void read();

	Json::Reader reader;
	Json::StyledWriter writer;
	Json::Value root;

	const std::string path;

	void addNewRun(
			const std::string& source,
			const std::string& type,
			const std::string& dir,
			const Poco::Timestamp& time,
			const std::string& builtFromDir = "");
};

}
}
#endif
