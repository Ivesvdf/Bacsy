#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <istream>
#include <sstream>
#include <list>
#include <map>
#include <stdexcept>
#include "stringUtils.h"

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
	string get(const string& key);
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

	template<typename T>
	T get(string section, string key)
	{
		if(sectionMap.count(section) == 0)
			throw NoSuchSectionException(section);

		const string val = sectionMap[section].get(StringUtils::rstrip(key, "[]"));


		Converter<T> c;
		return c.convert(val);
	}

	template<typename T>
	T get(string section, string key, const T& defaultVal)
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

};

#endif
