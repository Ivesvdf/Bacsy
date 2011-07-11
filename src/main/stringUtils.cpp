#include "stringUtils.h"

using std::string;

std::string StringUtils::rstrip(const std::string& source , const std::string& t)
{
	std::string str = source;
	return str.erase(str.find_last_not_of(t) + 1);
}

std::string StringUtils::lstrip(const std::string& source , const std::string& t)
{
	std::string str = source;
	return str.erase(0, source.find_first_not_of(t));
}

std::string StringUtils::strip(const std::string& source , const std::string& t)
{
	std::string str = source;
	return lstrip(rstrip(str, t), t);
}  
