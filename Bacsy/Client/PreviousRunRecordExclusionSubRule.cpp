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

#include <string>
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/StringUtils.h"
#include "Bacsy/Client/PreviousRunRecordExclusionSubRule.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;


PreviousRunRecordExclusionSubRule::PreviousRunRecordExclusionSubRule(
		PreviousRunRecord& prr,
		bool negated):
	ExclusionSubRule(negated),
	prr(prr)
{
}

bool PreviousRunRecordExclusionSubRule::matchWithoutNegate(
		const IFile& inputFile)
{
	if(!inputFile.isFile())
	{
		return false;
	}

	// Exclude when this file was previously backed up
	if(prr.wasFileBackedUp(inputFile.path()))
	{
		LOGI("TRUE! This file was backed up");
	}
	else
	{
		LOGI("FALSE! not backed up");
	}
	return prr.wasFileBackedUp(inputFile.path());
}

Bacsy::Rules::ExclusionSubRule* PreviousRunRecordExclusionSubRule::clone()
	const
{
	return new PreviousRunRecordExclusionSubRule(*this);
}

PreviousRunRecordExclusionSubRule::PreviousRunRecordExclusionSubRule(
		const PreviousRunRecordExclusionSubRule& copy):
	ExclusionSubRule(copy.getNegated()),
	prr(copy.prr)
{
	
}

}
}
