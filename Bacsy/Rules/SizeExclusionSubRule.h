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

#ifndef SIZE_EXCLUSION_SUB_RULE_H
#define SIZE_EXCLUSION_SUB_RULE_H

#include <string>
#include "Poco/File.h"
#include "Bacsy/Rules/ExclusionSubRule.h"

namespace Bacsy
{
namespace Rules
{

using namespace Common;


class SizeExclusionSubRule : public ExclusionSubRule
{
public:
	enum Operator { GREATER_THAN, LESS_THAN };

	SizeExclusionSubRule(const Poco::File::FileSize sizeInBytes, const Operator theOperator, bool negated);
	SizeExclusionSubRule(const SizeExclusionSubRule& copy);
	bool matchWithoutNegate(const IFile& inputFile);
	virtual ExclusionSubRule* clone() const;

	Poco::File::FileSize getSizeInBytes() const;
	Operator getOperator() const;
private:
	const Poco::File::FileSize sizeInBytes;
	const Operator theOperator;
};

}
}
#endif
