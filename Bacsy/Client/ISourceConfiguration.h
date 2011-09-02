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
 
#ifndef ISOURCECONFIGURATION_H
#define ISOURCECONFIGURATION_H

#include <vector>

#include "Bacsy/Rules/ExclusionRule.h"

namespace Bacsy
{

class ISourceConfiguration
{
public:
	enum PreferredOrderValue
	{
		PREFER_OTHER,
		PREFER_THIS,
	};

	typedef std::vector<PreferredOrderValue> PreferredOrder;

	virtual ~ISourceConfiguration() {
	}

	virtual std::string getName() const = 0;
	virtual std::vector<std::string> getIncludes() const = 0;
	virtual std::list<ExclusionRule>  getExcludes() const = 0;
	virtual unsigned int getPriority() const = 0;
	virtual unsigned int getMinBackups() const = 0;
	virtual unsigned int getMaxBackups() const = 0;
	virtual PreferredOrder getPreferredOrder() const = 0;
	virtual std::string getDistribution() const = 0;
	virtual std::string getTimerString() const = 0;
	virtual bool getEnabled() const = 0;
	virtual bool getDryPrintRun() const = 0;
	virtual std::string getHostIdentification() const = 0;
};

}

#endif
