/*
 * Copyright (C) 2011  Ives van der Flaas
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
#include "Bacsy/Server/JsonStoreIndex.h"

namespace Bacsy
{

TEST( StoreIndexTest, NoReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	JsonStoreIndex store(tmp.path());
	
	ASSERT_EQ("", store.getLastFullRun("aSource"));
	store.addNewFullRun("aSource", "aSource_1");
	ASSERT_EQ("aSource_1", store.getLastFullRun("aSource"));
	
	store.addNewDeltaRun("aSource", "aSource_2", "aSource_1");
	ASSERT_EQ("aSource_1", store.getLastFullRun("aSource"));

	ASSERT_EQ("aSource_1", store.getCorrespondingFullRunForDelta("aSource", "aSource_2"));

	ASSERT_FALSE(store.isDeltaRun("aSource", "aSource_1"));
	ASSERT_TRUE(store.isDeltaRun("aSource", "aSource_2"));

	store.addNewDeltaRun("aSource", "aSource_3", "aSource_1");
	store.addNewDeltaRun("aSource", "aSource_4", "aSource_1");
	store.addNewDeltaRun("aSource", "aSource_5", "aSource_1");
	store.addNewDeltaRun("aSource", "aSource_6", "aSource_1");

	store.addNewFullRun("aSource", "aSource_7");

	store.addNewDeltaRun("aSource", "aSource_8", "aSource_7");
	store.addNewDeltaRun("aSource", "aSource_9", "aSource_7");
	store.addNewDeltaRun("aSource", "aSource_10", "aSource_7");

	ASSERT_EQ("aSource_7", store.getCorrespondingFullRunForDelta("aSource", "aSource_8"));
	ASSERT_EQ("aSource_7", store.getCorrespondingFullRunForDelta("aSource", "aSource_9"));
	ASSERT_EQ("aSource_7", store.getCorrespondingFullRunForDelta("aSource", "aSource_10"));

	ASSERT_EQ("aSource_1", store.getCorrespondingFullRunForDelta("aSource", "aSource_4"));
	ASSERT_EQ("aSource_1", store.getCorrespondingFullRunForDelta("aSource", "aSource_5"));
	ASSERT_EQ("aSource_1", store.getCorrespondingFullRunForDelta("aSource", "aSource_6"));
}

TEST( StoreIndexTest, ReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	{
		JsonStoreIndex storer(tmp.path());

		storer.addNewFullRun("aSource", "aSource_1");
		storer.addNewDeltaRun("aSource", "aSource_2", "aSource_1");


		storer.addNewDeltaRun("aSource", "aSource_3", "aSource_1");
		storer.addNewDeltaRun("aSource", "aSource_4", "aSource_1");
		storer.addNewDeltaRun("aSource", "aSource_5", "aSource_1");
		storer.addNewDeltaRun("aSource", "aSource_6", "aSource_1");

		storer.addNewFullRun("aSource", "aSource_7");

		storer.addNewDeltaRun("aSource", "aSource_8", "aSource_7");
		storer.addNewDeltaRun("aSource", "aSource_9", "aSource_7");
		storer.addNewDeltaRun("aSource", "aSource_10", "aSource_7");
	}
	{
		JsonStoreIndex loader(tmp.path());

		ASSERT_EQ("aSource_7", loader.getLastFullRun("aSource"));

		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRunForDelta("aSource", "aSource_2"));

		ASSERT_FALSE(loader.isDeltaRun("aSource", "aSource_1"));
		ASSERT_TRUE(loader.isDeltaRun("aSource", "aSource_2"));

		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRunForDelta("aSource", "aSource_8"));
		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRunForDelta("aSource", "aSource_9"));
		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRunForDelta("aSource", "aSource_10"));

		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRunForDelta("aSource", "aSource_4"));
		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRunForDelta("aSource", "aSource_5"));
		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRunForDelta("aSource", "aSource_6"));
	}
}

}
