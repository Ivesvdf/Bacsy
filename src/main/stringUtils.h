#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <sstream>
#include <string>
#include <vector>

class StringUtils
{
public:
	static std::string lstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string rstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string strip(const std::string& input, const std::string& stripChars = " ");
	
	static std::vector<std::string> split(const std::string& input, const char delim);

	template<typename T>
	static std::string toString(const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template<typename T>
	static T fromString(const std::string& s)
	{
		std::stringstream ss;
		T t;
		ss << s;
		ss >> t;
		return t;
	}
};

#endif
