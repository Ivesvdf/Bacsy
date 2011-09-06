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
namespace Client
{

using namespace Common;
using namespace Rules;

class NoSuchSourceException: public runtime_error
{
public:
	NoSuchSourceException(string sourceName): runtime_error(string("No source by the name of " + sourceName + " exists. ")) {}
	virtual ~NoSuchSourceException() throw() {}

};


class CascadingSourceConfiguration : public CascadingFileConfiguration
{
	public:
		CascadingSourceConfiguration(const std::string& directory);

		std::list<std::string> getSources() const;

		class Section : public ISourceConfiguration
		{
		public:
			Section(const std::string& name, const CascadingSourceConfiguration& config);

			std::string getName() const;
			IncludeCollection getIncludes() const;
			ExcludeCollection getExcludes() const;
			unsigned int getPriority() const;
			unsigned int getMinBackups() const;
			unsigned int getMaxBackups() const;
			PreferredOrder getPreferredOrder() const;
			Distribution getDistribution() const;
			TimeTable getTimeTable() const;
			bool getEnabled() const;
			bool getDryPrintRun() const;
			std::string getHostIdentification() const;
		private:
			std::string name;
			const CascadingSourceConfiguration& config;
		};

		const Section getSource(const std::string& name) const;
};

}
}
#endif
