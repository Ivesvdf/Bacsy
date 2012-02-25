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

#ifndef BACSY_OPLAIN_TEXT_PREVIOUS_RUN_RECORD_H
#define BACSY_OPLAIN_TEXT_PREVIOUS_RUN_RECORD_H

#include <string>
#include <set>
#include "Bacsy/Common/PlainTextPreviousRunRecord.h"

namespace Bacsy
{
namespace Common
{

class OPlainTextPreviousRunRecord : public PlainTextPreviousRunRecord
{
	public:
		OPlainTextPreviousRunRecord(const std::string& recordfile);
		virtual bool wasFileBackedUp(const std::string& filename) const;

		virtual void setFileBackedUp(const std::string& filename);
		virtual void backupCompleted();
};

}
}

#endif
