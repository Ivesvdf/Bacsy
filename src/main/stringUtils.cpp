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

#include <sstream>
#include <algorithm>
#include <locale>
#include <functional>
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

std::vector<std::string> StringUtils::split(const std::string& input, const char delim)
{
	std::vector<std::string> elems;
	std::stringstream ss(input);
	std::string item;
	while(std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> StringUtils::split(const std::string& input, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t next = std::string::npos;
	size_t firstNew = 0;

	while(std::string::npos != (next = input.find(delim, firstNew)))
	{
		elems.push_back(input.substr(firstNew, next - firstNew));
		firstNew = next + delim.length();
	}

	// Push last one
	elems.push_back(input.substr(firstNew));

	return elems;
}

std::string StringUtils::toLower(const std::string& input)
{
	std::string copy(input.size(), ' ');
	std::transform(input.begin(), input.end(), copy.begin(), static_cast<int(*)(int)>(std::tolower)) ;
	return copy;
}

