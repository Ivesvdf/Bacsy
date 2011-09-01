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

#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/ArgParsingUtils.h"

namespace bacsy
{

void ArgParsingUtils::processDefinitions(const std::vector<std::string>& definitions, ConfigurationFile& conf)
{
	for(std::vector<std::string>::const_iterator it = definitions.begin(); 
			it != definitions.end();
			++it)
	{
		std::stringstream ss;

		const size_t bracketPos = it->find_first_of(']');

		if(bracketPos == std::string::npos)
		{
			LOGE("No bracket found; ignoring definition " + *it);
			continue;
		}

		ss << it->substr(0, bracketPos + 1) + "\n" + it->substr(bracketPos + 1);

		ConfigurationFile tmp(ss);
		conf.merge(tmp);
	}
}

}
