#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

class StringUtils
{
public:
	static std::string lstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string rstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string strip(const std::string& input, const std::string& stripChars = " ");
	
	static std::vector<std::string> split(const std::string& input, const char delim);
};

#endif
