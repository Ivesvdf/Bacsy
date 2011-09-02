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

#ifndef CASCADING_SOURCE_CONFIGURATION
#define CASCADING_SOURCE_CONFIGURATION

#include <list>
#include <fstream>
#include <limits>
#include <string>

#include "Bacsy/Client/ISourceConfiguration.h"
#include "Bacsy/Common/ConfigurationFile.h"
#include "Bacsy/Rules/ExclusionRule.h"
#include "Bacsy/Common/CascadingFileConfiguration.h"

namespace Bacsy
{

class CascadingSourceConfiguration : public CascadingFileConfiguration
{
	public:
		CascadingSourceConfiguration(const std::string& directory);

		std::list<std::string> getSources() const;
		const ISourceConfiguration& getSource(const std::string& name) const;
	private:
		class Section : public ISourceConfiguration
		{
		public:
			Section(const std::string& name, const CascadingSourceConfiguration& config);

			std::string getName() const;
			std::vector<std::string> getIncludes() const;
			std::list<ExclusionRule>  getExcludes() const;
			unsigned int getPriority() const;
			unsigned int getMinBackups() const;
			unsigned int getMaxBackups() const;
			std::string getPreferredOrder() const;
			std::string getDistribution() const;
			std::string getTimerString() const;
			bool getEnabled() const;
			bool getDryPrintRun() const;
			std::string getHostIdentification() const;
		private:
			std::string name;
			const CascadingSourceConfiguration& sourceFile;
		};

		template<typename T>
		T getCascadingSourceValue( 
				const std::string& section,
				const std::string& keyname, 
				const T& defaultValue = T()) const
		{
			return getCascadingValue(
					config,
					section,
					keyname,
					defaultValue);
		}
};

}
#endif
