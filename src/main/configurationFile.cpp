#include <string>
#include "woodcutter/woodcutter.h"
#include "configurationFile.h"
#include "stringUtils.h"

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
			currentSection = line.substr(1, len - 2);
			LOGI("Section found: " + currentSection);

			if(sectionMap.count(currentSection) != 0)
				throw SyntaxError("No two sections may have the same name (" + currentSection + ")." );

			sectionMap[currentSection] = Section();
		}
		// No section => key, val!
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

			sectionMap[currentSection].put(key,val);
		}
	}
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

string Section::get(const string& key)
{
	if(kvMap.count(key) == 0)
		throw NoSuchKeyException(key);

	return kvMap[key];
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
