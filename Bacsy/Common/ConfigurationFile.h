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

#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <istream>
#include <sstream>
#include <list>
#include <map>
#include <stdexcept>
#include "Bacsy/Common/StringUtils.h"

namespace Bacsy
{

using std::istream;
using std::list;
using std::map;
using std::string;
using std::runtime_error;

class SyntaxError: public runtime_error
{
public:
	SyntaxError(string message): runtime_error(message) {}
	virtual ~SyntaxError() throw() {}
};

class NoSuchSectionException: public runtime_error
{
public:
	NoSuchSectionException(string sectionName): runtime_error(string("No section by the name of " + sectionName + " exists. ")) {}
	virtual ~NoSuchSectionException() throw() {}
};

class NoSuchKeyException: public runtime_error
{
public:
	NoSuchKeyException(string keyName): runtime_error(string("No key by the name of " + keyName + " exists. ")) {}
	virtual ~NoSuchKeyException() throw() {}

};

class Section
{
private:
	typedef map<string, string> KeyValMap;
	KeyValMap kvMap;

public:
	void put(const string& key, const string& val);
	string get(const string& key) const;
	list<string> keys() const;
};

template<typename S>
class Converter
{
public:
	S convert(std::string in)
	{
		std::stringstream ss;
		S output;
		ss << StringUtils::strip(in, " \t");
		if(!(ss >> output) || !ss.eof())
			throw runtime_error("Value " + in + " cannot be converted.");
		return output;
	}
};

template <>
class Converter <std::string>
{
public:
	std::string convert(std::string in)
	{
		return in;
	}
};

class ConfigurationFile
{
private:
	typedef map<string, Section> SectionMap;	
	typedef SectionMap::iterator iterator;
	typedef SectionMap::const_iterator const_iterator;

	SectionMap sectionMap;
public:
	ConfigurationFile(istream& fromFile);

	/**
	 * @return a Linked list containing the names of all sections
	 */
	list<string> sections() const;
	list<string> keys(const std::string& section) const;

	/**
	 * Merges another configurationFile into this one. Other will override
	 * this!
	 */
	void merge(const ConfigurationFile& other);

	template<typename T>
	T get(string section, string key) const;

	template<typename T>
	T get(string section, string key, const T& defaultVal) const;

	void put(const string& section, const string& key, const string& val);

	void registerSection(const string& section);
};

template<typename T>
T ConfigurationFile::get(string section, string key) const 
{
	SectionMap::const_iterator it = sectionMap.find(section);
	if(it == sectionMap.end())
		throw NoSuchSectionException(section);

	const string val = it->second.get(StringUtils::rstrip(key, "[]"));

	Converter<T> c;
	return c.convert(val);
}

template<typename T>
T ConfigurationFile::get(string section, string key, const T& defaultVal) const
{
	try 
	{
		return get<T>(section, key);
	}
	catch(NoSuchKeyException e)
	{
		return defaultVal;
	}
}

}
#endif
