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

#ifndef CASCADING_STORE_CONFIGURATION
#define CASCADING_STORE_CONFIGURATION

#include <list>
#include <fstream>
#include <limits>
#include <string>
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/ConfigurationFile.h"
#include "Bacsy/Rules/ExclusionRule.h"
#include "Bacsy/Common/CascadingFileConfiguration.h"

namespace Bacsy
{

class CascadingStoreConfiguration : public CascadingFileConfiguration
{
	public:
		CascadingStoreConfiguration(const std::string& directory);

		std::list<std::string> getStores() const;
		unsigned int getMinPriorityForStoring(const std::string& input) const;
		std::string getLocation(const std::string& store) const;
		bool getAlwaysPresent(const std::string& store) const;
	private:
		template<typename T>
		T getCascadingStoreValue( 
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
