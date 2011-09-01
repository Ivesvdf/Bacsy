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

#include <string>
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/ConfigurationFile.h"
#include "Bacsy/Common/StringUtils.h"

namespace Bacsy
{

ConfigurationFile::ConfigurationFile(istream& fromSource)
{
	std::string line;
	std::string currentSection;

	while(std::getline(fromSource, line))
	{
		line = StringUtils::strip(line, " \t");
 
		const size_t len = line.length();

		// Ignore empty lines
		if(len == 0)
			continue;

		// Section found
		if((line[0] == '[') && (line[len-1] == ']') && (len > 2))
		{
			currentSection = StringUtils::strip(line.substr(1, len - 2), " \t");
			LOGI("Section found: " + currentSection);

			if(sectionMap.count(currentSection) != 0)
				throw SyntaxError("No two sections may have the same name (" + currentSection + ")." );

			registerSection(currentSection);
		}
		// Comments
		else if(line[0] == ';' || line[0] == '#')
		{
			// Comment... ignore
		}
		// No section, no comment => key, val!
		else
		{
			if(currentSection.empty())
				throw SyntaxError("A configuration file should start with a section. ");
			const size_t equalsPos = line.find_first_of('=');
			if(equalsPos == std::string::npos)
				throw SyntaxError("No equals sign in key-value line " + line);

			const string key = StringUtils::strip(line.substr(0, equalsPos), " \t");
			const string val = StringUtils::strip(line.substr(equalsPos+1), " \t");

			if(key.empty())
				throw SyntaxError("No empty keys allowed: " + line);
			if(val.empty())
				throw SyntaxError("No empty values allowed: " + line);

			LOGI("Key-val found: " + key + ":" + val);
			sectionMap[currentSection].put(key,val);
		}
	}
}

void ConfigurationFile::registerSection(const std::string& section)
{
	sectionMap[section] = Section();
}

void Section::put(const string& inputKey, const string& val)
{
	string key = inputKey;
	const int len = key.length();
	bool arrayKey = false;

	// Special key with [] suffix
	if(len > 2 && key[len-2] == '[' && key[len-1] == ']')
	{
		key = key.substr(0, len-2);
		arrayKey = true;
	}

	if(!arrayKey || kvMap.count(key) == 0)
	{
		kvMap[key] = val;
	}
	else if(arrayKey)
	{
		kvMap[key] += "\n" + val;
	}
}

string Section::get(const string& key) const 
{
	KeyValMap::const_iterator it = kvMap.find(key);

	if(it == kvMap.end())
		throw NoSuchKeyException(key);

	return it->second;
}

list<string> Section::keys() const
{
	std::list<std::string> rv;

	for(KeyValMap::const_iterator it = kvMap.begin(); it != kvMap.end(); it++)
	{
		rv.push_back(it->first);
	}

	return rv;
}

list<string> ConfigurationFile::sections() const
{
	list<string> rv;

	for(SectionMap::const_iterator it = sectionMap.begin(); it != sectionMap.end(); it++)
	{
		rv.push_back(it->first);
	}

	return rv;
}

list<string> ConfigurationFile::keys(const std::string& section) const
{
	SectionMap::const_iterator it = sectionMap.find(section);
	if(it == sectionMap.end())
		throw NoSuchSectionException(section);

	return it->second.keys();
}


void ConfigurationFile::put(const string& section, const string& key, const string& val)
{
	SectionMap::iterator it = sectionMap.find(section);
	if(it == sectionMap.end())
		throw NoSuchSectionException(section);

	it->second.put(key,val);
}

void ConfigurationFile::merge(const ConfigurationFile& other)
{
	std::list<std::string> sections = other.sections();
			
	for(std::list<std::string>::iterator sectionIt = sections.begin();
			sectionIt != sections.end();
			++sectionIt)
	{
		if(sectionMap.count(*sectionIt) == 0)
		{
			registerSection(*sectionIt);
		}

		std::list<std::string> keys = other.keys(*sectionIt);

		for(std::list<std::string>::iterator keyIterator = keys.begin();
				keyIterator != keys.end();
				++keyIterator)
		{
			put(*sectionIt, *keyIterator, other.get<std::string>(*sectionIt, *keyIterator));
		}
	}
}

}
