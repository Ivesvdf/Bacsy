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
#include "ConfigurationFile.h"
#include "ExclusionRule.h"


class CascadingFileConfiguration
{
	public:
		CascadingFileConfiguration(const std::string& directory);

		std::list<std::string> getSources() const;
		std::list<std::string> getStores() const;
		std::vector<std::string> getIncludes(const std::string& source) const;
		std::list<ExclusionRule>  getExcludes(const std::string& source) const;
		unsigned int getPriority(const std::string& source) const;
		unsigned int getMinBackups(const std::string& source) const;
		unsigned int getMaxBackups(const std::string& source) const;
		std::string getPreferredOrder(const std::string& source) const;
		std::string getDistribution(const std::string& source) const;
		std::string getTimerString(const std::string& source) const;
		std::string getHostIdentification(const std::string& source) const;
		std::string getLocation(const std::string& store) const;
		bool getAlwaysPresent(const std::string& store) const;
		unsigned int getMinPriorityForStoring(const std::string& input) const;

	private:
		template<typename T>
		T getCascadingStoreValue( 
				const std::string& section,
				const std::string& keyname, 
				const T& defaultValue = T()) const
		{
			return getCascadingValue(
					storeConfig,
					section,
					keyname,
					defaultValue);
		}
		
		template<typename T>
		T getCascadingSourceValue( 
				const std::string& section,
				const std::string& keyname, 
				const T& defaultValue = T()) const
		{
			return getCascadingValue(
					sourceConfig,
					section,
					keyname,
					defaultValue);
		}

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


		std::ifstream inputSourceStream;	
		std::ifstream inputStoreStream;	
		ConfigurationFile sourceConfig;
		ConfigurationFile storeConfig;
		const std::string globalSectionName;
};

#endif
