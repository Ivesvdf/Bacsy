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

#ifndef BACSY_PREVIOUS_RUN_RECORD_H
#define BACSY_PREVIOUS_RUN_RECORD_H

#include <string>

namespace Bacsy
{
namespace Client
{

/**
 * Interface class that must be implemented so clients know what files were
 * backed up in the previous run.
 */
class PreviousRunRecord
{
	public:
		virtual ~PreviousRunRecord(){};

		// Read methods
		virtual bool wasFileBackedUp(const std::string& filename) const=0;

		// Write methods
		/**
		 * Records that the file filename was backed up in this backup run.
		 * This can mean that it was actually sent over the wire or that the
		 * remote version was still up to date.
		 */
		virtual void setFileBackedUp(const std::string& filename)=0;

		/**
		 * This function is called when no more instances of setFileBackup()
		 * will be called this run. E.g. so this class can store data.
		 */
		virtual void backupCompleted()=0;
};

}
}

#endif
