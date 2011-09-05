/*
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

#ifndef ISTORECONFIGURATION_H
#define ISTORECONFIGURATION_H

#include <vector>
#include <list>

namespace Bacsy
{
namespace Server
{

using namespace Common;

class IStoreConfiguration
{
public:
	virtual ~IStoreConfiguration() {
	}

	virtual std::string getName() const = 0;
	virtual unsigned int getMinPriorityForStoring() const = 0;
	virtual std::string getLocation() const = 0;
	virtual bool getAlwaysPresent() const = 0;
	virtual bool getEnabled() const = 0;
};

}
}

#endif
