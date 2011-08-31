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

#include "ExclusionStringParser.h"

namespace bacsy
{

std::list<std::string> ExclusionStringParser::parse(const std::string& input)
{
	size_t pos = 0;
	std::list<std::string> rv;

	while(pos < input.length())
	{
		if(input[pos] == '"') // Quoted string
		{
			std::string tmp;
			bool prevWasEscape = false;

			pos++;
			while(pos < input.length())
			{
				const char c = input[pos];
				
				if(c == '\\' && !prevWasEscape)
				{
					prevWasEscape = true;
				}
				else if(c == '\\' && prevWasEscape)
				{
					tmp.push_back('\\');
					prevWasEscape = false;
				}
				else if(c == '"' && prevWasEscape)
				{
					tmp.push_back('"');
					prevWasEscape = false;
				}
				else if(c == '"' && !prevWasEscape)
				{
					break;
				}
				else
				{
					if(prevWasEscape)
					{
						tmp.push_back('\\');
					}

					tmp.push_back(c);
					prevWasEscape = false;
				}

				pos++;
			}

			rv.push_back(tmp);
		}
		else if(input[pos] == '!')
		{
			rv.push_back("!");
		}
		else // Normal text chunk
		{
			bool prevWasEscape = false;
			std::string tmp;
			while(pos < input.length())
			{
				const char c = input[pos];
				
				if(c == '\\')
				{
					prevWasEscape = true;
				}
				else if(c == ' ' && prevWasEscape)
				{
					tmp.push_back(' ');
					prevWasEscape = false;
				}
				else if(c == ' ')
				{
					break;
				}
				else
				{
					if(prevWasEscape)
					{
						tmp.push_back('\\');
					}
					tmp.push_back(input[pos]);
					prevWasEscape = false;
				}

				pos++;
			}
			
			rv.push_back(tmp);
		}


		do
		{
			pos++;
		}
		while(pos < input.length() && input[pos] == ' ');

	}

	return rv;
}

}
