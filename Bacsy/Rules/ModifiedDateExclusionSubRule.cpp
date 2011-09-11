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
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Rules/ModifiedDateExclusionSubRule.h"

namespace Bacsy
{
namespace Rules
{

using namespace Common;


ModifiedDateExclusionSubRule::ModifiedDateExclusionSubRule(
		const Poco::Timestamp& timestamp,
		const Operator theOperator,
		bool negated):
	ExclusionSubRule(negated),
	timestamp(timestamp),
	theOperator(theOperator)
{
}

bool ModifiedDateExclusionSubRule::matchWithoutNegate(const IFile& inputFile)
{
	if(!inputFile.isFile())
	{
		return false;
	}

	if(theOperator == OLDER_THAN)
		return inputFile.getLastModified() < timestamp;
	else
		return inputFile.getLastModified() > timestamp;
}

ExclusionSubRule* ModifiedDateExclusionSubRule::clone() const
{
	return new ModifiedDateExclusionSubRule(*this);
}

ModifiedDateExclusionSubRule::ModifiedDateExclusionSubRule(
		const ModifiedDateExclusionSubRule& copy):
	ExclusionSubRule(copy.getNegated()),
	timestamp(copy.timestamp),
	theOperator(copy.theOperator)
{
	
}

Poco::Timestamp ModifiedDateExclusionSubRule::getTimestamp() const
{
	return timestamp;
}

ModifiedDateExclusionSubRule::Operator ModifiedDateExclusionSubRule::getOperator() const
{
	return theOperator;
}

}
}
