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

#include <gtest/gtest.h>
#include "Poco/TemporaryFile.h"
#include "Bacsy/Client/IPlainTextPreviousRunRecord.h"
#include "Bacsy/Client/OPlainTextPreviousRunRecord.h"

using namespace Bacsy::Client;

TEST( PlainTextPreviousRunRecordTest, FirstReadNothingInIt )
{
	Poco::TemporaryFile f;
	IPlainTextPreviousRunRecord rec(f.path());

	ASSERT_FALSE(rec.wasFileBackedUp(""));
	ASSERT_FALSE(rec.wasFileBackedUp("a"));
	ASSERT_FALSE(rec.wasFileBackedUp("*"));
	ASSERT_FALSE(rec.wasFileBackedUp("a")); // Test same thing twice... 
}

TEST( PlainTextPreviousRunRecordTest, WriteReadTest )
{
	Poco::TemporaryFile f;

	OPlainTextPreviousRunRecord writeRec(f.path());
	const std::string s1 ="C:/users/Ives.docx"; 
	ASSERT_FALSE(writeRec.wasFileBackedUp(s1));
	writeRec.setFileBackedUp(s1);
	ASSERT_TRUE(writeRec.wasFileBackedUp(s1));

	const std::string s2 ="/home/naomi/docs/file1"; 
	ASSERT_FALSE(writeRec.wasFileBackedUp(s2));
	writeRec.setFileBackedUp(s2);
	ASSERT_TRUE(writeRec.wasFileBackedUp(s2));

	writeRec.backupCompleted();

	IPlainTextPreviousRunRecord readRec(f.path());
	ASSERT_TRUE(readRec.wasFileBackedUp(s1));
	std::string s1cap ="C:/Users/Ives.docx"; 
	ASSERT_FALSE(readRec.wasFileBackedUp(s1cap)); // Should be caps sensitive!
	ASSERT_FALSE(readRec.wasFileBackedUp("")); 

	ASSERT_FALSE(readRec.wasFileBackedUp("/home/ives/somepath.txt")); 

	ASSERT_TRUE(readRec.wasFileBackedUp(s2));

	// Now a second backup starts where naomi no longer backs up her files
	// This is to test the file is fully erased when a new run is started
	{
		OPlainTextPreviousRunRecord writeRec(f.path());
		ASSERT_FALSE(writeRec.wasFileBackedUp(s1));
		writeRec.setFileBackedUp(s1);
		ASSERT_TRUE(writeRec.wasFileBackedUp(s1));

		writeRec.backupCompleted();

		IPlainTextPreviousRunRecord readRec(f.path());
		ASSERT_TRUE(readRec.wasFileBackedUp(s1));
		ASSERT_FALSE(readRec.wasFileBackedUp(s1cap)); // Should be caps sensitive!
		ASSERT_FALSE(readRec.wasFileBackedUp("")); 

		ASSERT_FALSE(readRec.wasFileBackedUp("/home/ives/somepath.txt")); 

		// File was not backed up in last run
		ASSERT_FALSE(readRec.wasFileBackedUp(s2)); 
	}
}
