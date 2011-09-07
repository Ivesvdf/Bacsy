/*
 * Copyright (C) 2011  Ives van der Flaas
 * Copyright (C) 2011  Nathan Samson
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
#include <set>
#include <fstream>
#include <limits>
#include <string>
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/ConfigurationFile.h"
#include "Bacsy/Rules/ExclusionRule.h"

namespace Bacsy
{
namespace Common
{

class CascadingFileConfiguration
{
	public:
		CascadingFileConfiguration(const std::string& filename);

		ConfigurationFile& getConfig();
		const ConfigurationFile& getConfig() const;	
		bool isLoaded() const;

	protected:
		template<typename T>
		T getCascadingValue(
				const std::string& section,
				const std::string& keyname, 
				const T& defaultValue = T()) const;

		static bool toBool(const std::string& input);
		virtual std::set<std::string> getValidKeys() const = 0;

		std::ifstream inputStream;	
		const bool configIsLoaded;
		ConfigurationFile config;
		const std::string globalSectionName;
		void checkKeys() const;
};

template<typename T>
T CascadingFileConfiguration::getCascadingValue(
		const std::string& section,
		const std::string& keyname, 
		const T& defaultValue) const
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

}
}
#endif
