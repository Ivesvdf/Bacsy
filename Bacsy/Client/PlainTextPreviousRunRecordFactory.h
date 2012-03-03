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

#ifndef BACSY_PLAIN_TEXT_PREVIOUS_RUN_RECORD_FACTORY_H
#define BACSY_PLAIN_TEXT_PREVIOUS_RUN_RECORD_FACTORY_H

#include <string>
#include "Bacsy/Client/PreviousRunRecordFactory.h"

namespace Bacsy
{
namespace Client
{


/**
 * Interface class that makes new PreviousRunRecords
 */
class PlainTextPreviousRunRecordFactory :
	public Bacsy::Client::PreviousRunRecordFactory
{
	public:
		PlainTextPreviousRunRecordFactory(const std::string& basepath);

		/**
		 * Creates a new PreviousRunRecord that contains data on a previous
		 * run. This object will be deleted when no longer needed by the
		 * Client.
		 */
		virtual PreviousRunRecord* readPreviousRunRecord(
				const std::string& sourceName);

		/**
		 * Creates a new PreviousRunRecord that will be written to so later
		 * runs will know something about what was backed up this run. 
		 */
		virtual PreviousRunRecord* newPreviousRunRecord(
				const std::string& sourceName);

	private:
		const std::string basepath;
};

}
}

#endif

