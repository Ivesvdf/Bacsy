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

#ifndef BACSY_PATH_EXCLUSION_SUB_RULE_H
#define BACSY_PATH_EXCLUSION_SUB_RULE_H

#include <string>
#include "Bacsy/Rules/ExclusionSubRule.h"

namespace Bacsy
{
namespace Rules
{

using namespace Common;


class PathExclusionSubRule : public ExclusionSubRule
{
public:
	PathExclusionSubRule(const std::string path, bool negated);
	PathExclusionSubRule(const PathExclusionSubRule& copy);
	bool matchWithoutNegate(const IFile& inputFile);
	virtual ExclusionSubRule* clone() const;

	virtual std::string toString() const 
	{ return "PathExclusionSubRule: " + path; }

private:
	const std::string path;
};

}
}
#endif
