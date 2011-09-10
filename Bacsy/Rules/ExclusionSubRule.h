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

#ifndef BACSY_EXCLUSION_SUB_RULE_H
#define BACSY_EXCLUSION_SUB_RULE_H

#include "Poco/File.h"
#include "Bacsy/Common/IFile.h"

namespace Bacsy
{
namespace Rules
{

using namespace Common;


class ExclusionSubRule
{
public:
	ExclusionSubRule(bool negated);
	virtual ~ExclusionSubRule();
	virtual bool matchWithoutNegate(const IFile& inputFile)=0;
	bool match(const IFile& inputFile);
	virtual ExclusionSubRule* clone() const=0;
	bool getNegated() const { return negated; }

private:
	const bool negated;
};

}
}
#endif
