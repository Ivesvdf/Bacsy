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
#include "Bacsy/Server/IStoreConfiguration.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class NoSuchStoreException: public runtime_error
{
public:
	NoSuchStoreException(string storeName): runtime_error(string("No store by the name of " + storeName + " exists. ")) {}
	virtual ~NoSuchStoreException() throw() {}

};

class CascadingStoreConfiguration : public CascadingFileConfiguration
{
	public:
		CascadingStoreConfiguration(const std::string& directory);

		std::list<std::string> getStores() const;

		class Section : public IStoreConfiguration
		{
		public:
			Section(const std::string& name,
			        const CascadingStoreConfiguration& config);

			std::string getName() const;
			unsigned int getMinPriorityForStoring() const;
			std::string getLocation() const;
			bool getAlwaysPresent() const;
			bool getEnabled() const;
		private:
			std::string name;
			const CascadingStoreConfiguration& storeConfig;
		};

		const Section getStore(const std::string& name) const;

	protected:
		std::set<std::string> getValidKeys() const;
};

}
}
#endif
