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

#include <fstream>
#include "woodcutter/woodcutter.h"
#include "Bacsy/Common/IPlainTextPreviousRunRecord.h"

namespace Bacsy
{
namespace Common
{

IPlainTextPreviousRunRecord::IPlainTextPreviousRunRecord(const std::string& recordfile)
{
	std::ifstream readstream(recordfile.c_str(), std::ifstream::in);

	if(readstream)
	{
		std::string line;
		while(std::getline(readstream, line))
		{
			records.insert(line);
		}
	}
	else
	{
		LOGI("Not reading from file " + recordfile + " because file could not"
				" be read from.");
	}
}

bool IPlainTextPreviousRunRecord::wasFileBackedUp(const std::string& filename) const
{
	return records.count(filename) > 0;
}

void IPlainTextPreviousRunRecord::setFileBackedUp(const std::string& filename)
{
	records.insert(filename);
}

void IPlainTextPreviousRunRecord::backupCompleted()
{

}

}
}
