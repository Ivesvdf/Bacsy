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
#include "Bacsy/Common/Utils.h"
#include "Bacsy/Server/StoreManager.h"

namespace Bacsy
{
namespace Server
{

using namespace Common;

class StorenameToStorer
{
public:
	StorenameToStorer(const CascadingStoreConfiguration& configuration): configuration(configuration) {}
	Store* operator()(const std::string& storeName)
	{
		return new Store(configuration.getStore(storeName));
	}

private:
	const CascadingStoreConfiguration& configuration;
};

StoreManager::StoreManager(const CascadingStoreConfiguration& configuration):
	configuration(configuration),
	stores(buildStores(configuration))
{

}

StoreManager::~StoreManager()
{
	std::for_each(stores.begin(), stores.end(), ObjectDeleter());
}

std::list<Store*> StoreManager::buildStores(const CascadingStoreConfiguration& configuration)
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

bool StoreManager::notReadyForStoring(const Store* store)
{
	return !store->readyForStoring();
}

std::list<Store*> StoreManager::getStores(unsigned int minPriority) const
{
	std::list<Store*> copy(stores);
	copy.remove_if(StorePriorityLower(minPriority));
	copy.remove_if(notReadyForStoring);

	return copy;
}

}
}
