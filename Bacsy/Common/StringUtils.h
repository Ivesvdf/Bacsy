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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <sstream>
#include <string>
#include <vector>

namespace Bacsy
{
namespace Common
{


class StringUtils
{
public:
	class Stripper
	{
	public:
		Stripper(const std::string& toStrip = " ");
		std::string operator()(const std::string& input);
	private:
		const std::string toStrip;
	};

	static std::string lstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string rstrip(const std::string& input, const std::string& stripChars = " ");
	static std::string strip(const std::string& input, const std::string& stripChars = " ");
	
	static std::vector<std::string> split(const std::string& input, const char delim);
	static std::vector<std::string> split(const std::string& input, const std::string& delim);
	

	static std::string toLower(const std::string& input);

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

}
}

#endif
