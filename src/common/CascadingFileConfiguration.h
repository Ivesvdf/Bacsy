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

#ifndef CASCADING_FILE_CONFIGURATION
#define CASCADING_FILE_CONFIGURATION

#include <list>
#include <fstream>
#include <limits>
#include <string>
#include "woodcutter/woodcutter.h"
#include "common/ConfigurationFile.h"
#include "rules/ExclusionRule.h"

namespace bacsy
{

class CascadingFileConfiguration
{
	public:
		CascadingFileConfiguration();

	protected:
		template<typename T>
		T getCascadingValue( 
				const ConfigurationFile& config,
				const std::string& section,
				const std::string& keyname, 
				const T& defaultValue = T()) const
		{
			T rv = defaultValue;
			try
			{
				rv = config.get<T>(globalSectionName, keyname, rv);
			}
			catch(NoSuchSectionException& e)
			{
				// Ignore, global sections aren't obligatory
			}

			rv = config.get<T>(section, keyname, rv);

			return rv;
		}

		static bool toBool(const std::string& input);

		const std::string globalSectionName;

};

}
#endif
