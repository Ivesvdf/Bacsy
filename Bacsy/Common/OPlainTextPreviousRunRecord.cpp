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
#include "Bacsy/Common/OPlainTextPreviousRunRecord.h"

namespace Bacsy
{
namespace Common
{

OPlainTextPreviousRunRecord::OPlainTextPreviousRunRecord(const std::string& recordfile):
	PlainTextPreviousRunRecord(recordfile)
{

}

bool OPlainTextPreviousRunRecord::wasFileBackedUp(const std::string& filename) const
{
	return records.count(filename) > 0;
}

void OPlainTextPreviousRunRecord::setFileBackedUp(const std::string& filename)
{
	records.insert(filename);
}

void OPlainTextPreviousRunRecord::backupCompleted()
{
	std::ofstream readstream(recordfile.c_str());

	char newline = '\n';

	for(std::set<std::string>::const_iterator it = records.begin();
			it != records.end();
			++it)
	{
		readstream.write(it->c_str(), it->length());
		readstream.write(&newline, 1);
	}

	readstream.close();
}

}
}
