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

#include <limits>
#include "Poco/File.h"
#include "Poco/Glob.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "woodcutter/woodcutter.h"
#include "target.h"

template<typename T>
T getCascadingValue(const std::string& globalSection, 
		const ConfigurationFile& localConfig, const std::string& localSection,
		const std::string& keyname, const T& defaultValue = T())
{
	T rv = defaultValue;

	rv = localConfig.get<T>(globalSection, keyname, rv);
	rv = localConfig.get<T>(localSection, keyname, rv);

	return rv;
}

Target::Target(std::string section, const ConfigurationFile& config):
	name(section),
	globalSection("global"),
	includes(StringUtils::split(getCascadingValue<std::string>(
					globalSection, config, section, "Include"), '\n')),
	excludes(StringUtils::split(getCascadingValue<std::string>(
					globalSection, config, section, "Exclude"), '\n')),
	priority(getCascadingValue<unsigned int>(
				globalSection, config, section, "Priority")),
	minBackups(getCascadingValue<unsigned int>(
				globalSection, config, section, "MinBackups", 1)),
	maxBackups(getCascadingValue<unsigned int>(
				globalSection,
				config,
				section,
				"MaxBackups",
				std::numeric_limits<unsigned int>::max())),
	preferredOrder(getCascadingValue<std::string>(
				globalSection, config, section, "PreferredOrder", "this, other")),
	distribution(getCascadingValue<std::string>(
				globalSection, config, section, "Distribution", "focus"))
{
	for(std::vector<string>::const_iterator it = excludes.begin();
			it != excludes.end();
			it++)
	{
		const std::string& subject = *it;

		if(subject.find('/') != std::string::npos || 
				subject.find('\\') != std::string::npos)
		{
			// Not a glob
			pathExcludes.insert(StringUtils::rstrip(subject, "/\\"));
		}
		else
		{
			// glob
			globExcludes.push_back(subject);
		}
	}
}

void Target::backupPath(const Poco::File& path) const
{
	std::string pathString = path.path();
	LOGI("Filename = " + pathString);

	if(pathExcludes.count(StringUtils::rstrip(pathString, "/\\")) == 1)
	{
		LOGI("Is in excludes -- not exploring!");
		return;
	}

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

	const std::string localFile = pathString.substr(pathString.find_last_of("/\\")+1);
	for(std::vector<std::string>::const_iterator it = globExcludes.begin();
			it != globExcludes.end();
			it++)
	{
		Poco::Glob glob(*it);
		if(glob.match(localFile))
		{
			LOGI("Not continuing; exclude glob " + *it + " matched.");
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
			backupPath(*it);
		}
	}
	else // Is file
	{
		LOGI("Path is a file -- not expanding.");
		LOGI("Modified date = " + StringUtils::toString<Poco::Timestamp::UtcTimeVal>(path.getLastModified().utcTime()/10000000));
	}

}

void Target::start()
{
	LOGI("Starting target " + name);


	for( std::vector<std::string>::const_iterator it = includes.begin();
			it != includes.end();
			it++)
	{
		backupPath(Poco::File(*it));
	}

}
