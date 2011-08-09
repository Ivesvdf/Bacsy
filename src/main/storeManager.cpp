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

#include <functional>
#include <fstream>
#include <algorithm>
#include "utils.h"
#include "storeManager.h"

class StorenameToStorer
{
public:
	StorenameToStorer(const CascadingFileConfiguration& configuration): configuration(configuration) {}
	Store* operator()(const std::string& storeName)
	{
		return new Store(storeName, configuration);
	}

private:
	const CascadingFileConfiguration& configuration;
};

StoreManager::StoreManager(const CascadingFileConfiguration& configuration):
	configuration(configuration),
	stores(buildStores(configuration))
{

}

StoreManager::~StoreManager()
{
	std::for_each(stores.begin(), stores.end(), DeleteObject());
}

std::list<Store*> StoreManager::buildStores(const CascadingFileConfiguration& configuration)
{
	std::list<std::string> storeNames = configuration.getStores();
	
	std::list<Store*> stores;
	stores.resize(storeNames.size());


	StorenameToStorer storenameToStore(configuration); 

	std::transform(
			storeNames.begin(),
			storeNames.end(),
			stores.begin(),
			storenameToStore);
			
	return stores;
}

size_t StoreManager::getStoreCount() const
{
	return stores.size();
}

class StorePriorityLower
{
public:
	StorePriorityLower(unsigned int priority):
		priority(priority)
	{}

	bool operator()(Store* store)
	{
		return store->getMinPriorityForStoring() < priority;
	}
private:
	unsigned int priority;
};

std::list<Store*> StoreManager::getStores(unsigned int minPriority) const
{
	std::list<Store*> copy(stores);
	copy.remove_if(StorePriorityLower(minPriority));

	return copy;
}
