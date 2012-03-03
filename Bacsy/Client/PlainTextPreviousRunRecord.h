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

#ifndef BACSY_PLAIN_TEXT_PREVIOUS_RUN_RECORD_H
#define BACSY_PLAIN_TEXT_PREVIOUS_RUN_RECORD_H

#include <string>
#include <set>
#include "Bacsy/Client/PreviousRunRecord.h"

namespace Bacsy
{
namespace Client
{

/**
 * See IPlainTextPreviousRunRecord and OPlainTextPreviousRunRecord to
 * respectively read and write PlainTextPreviousRunRecords.
 */
class PlainTextPreviousRunRecord : public PreviousRunRecord
{
	public:
		PlainTextPreviousRunRecord(const std::string& recordfile);
		PlainTextPreviousRunRecord();
		virtual ~PlainTextPreviousRunRecord();
		virtual bool wasFileBackedUp(const std::string& filename) const = 0;

		virtual void setFileBackedUp(const std::string& filename) = 0;
		virtual void backupCompleted() = 0;

	protected:
		std::set<std::string> records;
		std::string recordfile;
};

}
}

#endif
