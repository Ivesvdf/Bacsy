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

#include <string>
#include "StringUtils.h"
#include "PathExclusionSubRule.h"

namespace bacsy
{

PathExclusionSubRule::PathExclusionSubRule(const std::string path, bool negated):
	ExclusionSubRule(negated),
	path(path)
{
}

bool PathExclusionSubRule::matchWithoutNegate(const Poco::File& inputFile)
{
	return StringUtils::rstrip(path, "/\\") == StringUtils::rstrip(inputFile.path(), "/\\");
}

ExclusionSubRule* PathExclusionSubRule::clone() const
{
	return new PathExclusionSubRule(*this);
}

PathExclusionSubRule::PathExclusionSubRule(const PathExclusionSubRule& copy):
	ExclusionSubRule(copy.getNegated()),
	path(copy.path)
{
	
}

}
