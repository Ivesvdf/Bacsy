/*
 * Copyright (C) 2011  Nathan Samson
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
 
#ifndef ISOURCECONFIGURATION_H
#define ISOURCECONFIGURATION_H

#include <vector>
#include <list>

#include "Bacsy/Common/MinVersions.h"
#include "Bacsy/Common/MaxVersions.h"
#include "Bacsy/Client/TimeSchedule.h"
#include "Bacsy/Rules/ExclusionRule.h"

namespace Bacsy
{
namespace Client
{

using namespace Rules;


class ISourceConfiguration
{
public:
	enum PreferredOrderValue
	{
		PREFER_OTHER,
		PREFER_THIS,
	};

	typedef std::vector<PreferredOrderValue> PreferredOrder;

	enum Distribution
	{
		DISTRIBUTION_FOCUS,
		DISTRIBUTION_SPREAD,
	};

	typedef std::list<TimeSchedule> TimeTable;

	virtual ~ISourceConfiguration() 
	{
	}

	typedef std::vector<std::string> IncludeCollection;
	typedef std::list<ExclusionRule> ExcludeCollection;

	virtual std::string getName() const = 0;
	virtual IncludeCollection getIncludes() const = 0;
	virtual ExcludeCollection getExcludes() const = 0;
	virtual unsigned int getPriority() const = 0;
	virtual unsigned int getMinBackups() const = 0;
	virtual unsigned int getMaxBackups() const = 0;
	virtual MinVersions getMinVersions() const = 0;
	virtual MaxVersions getMaxVersions() const = 0;
	virtual PreferredOrder getPreferredOrder() const = 0;
	virtual Distribution getDistribution() const = 0;
	virtual TimeTable getTimeTable() const = 0;
	virtual bool getEnabled() const = 0;
	virtual bool getDryPrintRun() const = 0;
	virtual std::string getHostIdentification() const = 0;
};

}
}

#endif
