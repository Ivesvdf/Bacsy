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

#ifndef CASCADING_SOURCE_CONFIGURATION
#define CASCADING_SOURCE_CONFIGURATION

#include <list>
#include <fstream>
#include <limits>
#include <string>
#include "woodcutter/woodcutter.h"
#include "ConfigurationFile.h"
#include "ExclusionRule.h"
#include "CascadingFileConfiguration.h"

namespace bacsy
{

class CascadingSourceConfiguration : public CascadingFileConfiguration
{
	public:
		CascadingSourceConfiguration(const std::string& directory);

		std::list<std::string> getSources() const;
		std::vector<std::string> getIncludes(const std::string& source) const;
		std::list<ExclusionRule>  getExcludes(const std::string& source) const;
		unsigned int getPriority(const std::string& source) const;
		unsigned int getMinBackups(const std::string& source) const;
		unsigned int getMaxBackups(const std::string& source) const;
		std::string getPreferredOrder(const std::string& source) const;
		std::string getDistribution(const std::string& source) const;
		std::string getTimerString(const std::string& source) const;
		bool getEnabled(const std::string& source) const;
		bool getDryPrintRun(const std::string& source) const;
		std::string getHostIdentification(const std::string& source) const;
		bool isLoaded() const;

		const ConfigurationFile& getConfig() const;
		ConfigurationFile& getConfig();

		ConfigurationFile& getSourceConfig();
	private:
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

		std::ifstream inputSourceStream;	

		const bool isSourceFileLoaded;

		ConfigurationFile sourceConfig;
		const std::string globalSectionName;
};

}
#endif
