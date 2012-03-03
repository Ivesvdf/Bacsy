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

#include "Bacsy/Client/PlainTextPreviousRunRecordFactory.h"
#include "Bacsy/Client/IPlainTextPreviousRunRecord.h"
#include "Bacsy/Client/OPlainTextPreviousRunRecord.h"

namespace Bacsy
{
namespace Client
{

PlainTextPreviousRunRecordFactory::PlainTextPreviousRunRecordFactory(
		const std::string& basepath) : basepath(basepath)
{

}

PreviousRunRecord* PlainTextPreviousRunRecordFactory::readPreviousRunRecord(
				const std::string& sourceName)
{
	return new Bacsy::Client::IPlainTextPreviousRunRecord(
			basepath + "/" + sourceName + ".dat");
}

PreviousRunRecord* PlainTextPreviousRunRecordFactory::newPreviousRunRecord(
				const std::string& sourceName)
{
	return new Bacsy::Client::OPlainTextPreviousRunRecord(
			basepath + "/" + sourceName + ".dat");
}

}
}
