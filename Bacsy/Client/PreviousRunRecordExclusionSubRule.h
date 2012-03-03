/*
 * Copyright (C) 2012  Ives van der Flaas
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

#ifndef BACSY_PREVIOUS_RUN_RECORD_EXCLUSION_SUB_RULE_H
#define BACSY_PREVIOUS_RUN_RECORD_EXCLUSION_SUB_RULE_H

#include <string>
#include "Poco/File.h"
#include "Bacsy/Rules/ExclusionSubRule.h"
#include "Bacsy/Client/PreviousRunRecord.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;


class PreviousRunRecordExclusionSubRule : public Bacsy::Rules::ExclusionSubRule
{
public:

	PreviousRunRecordExclusionSubRule(
			PreviousRunRecord& prr,
			bool negated);
	PreviousRunRecordExclusionSubRule(
			const PreviousRunRecordExclusionSubRule& copy);
	bool matchWithoutNegate(const IFile& inputFile);
	virtual ExclusionSubRule* clone() const;


	virtual std::string toString() const 
	{ return "PreviousRunRecordExclusionSubRule"; }
private:
	PreviousRunRecord& prr;
};

}
}
#endif
