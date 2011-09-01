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

#ifndef STORE_MANAGER_H
#define STORE_MANAGER_H

#include <list>
#include "Bacsy/Server/CascadingStoreConfiguration.h"
#include "Bacsy/Server/Store.h"

namespace Bacsy
{

class StoreManager
{
public:
	StoreManager(const CascadingStoreConfiguration& configuration);
	~StoreManager();

	size_t getStoreCount() const;
	std::list<Store*> getStores(unsigned int minPriority) const;
private:
	static std::list<Store*> buildStores(const CascadingStoreConfiguration& configuration);
	static bool notReadyForStoring(const Store* store);
	const CascadingStoreConfiguration& configuration;
	std::list<Store*> stores;
};

}
#endif
