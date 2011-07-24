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
#include "jsonStoreIndex.h"

TEST( StoreIndexTest, NoReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	JsonStoreIndex store(tmp.path());
	
	ASSERT_EQ("", store.getLastFullRun("aTarget"));
	store.addNewFullRun("aTarget", "aTarget_1");
	ASSERT_EQ("aTarget_1", store.getLastFullRun("aTarget"));
	
	store.addNewDeltaRun("aTarget", "aTarget_2", "aTarget_1");
	ASSERT_EQ("aTarget_1", store.getLastFullRun("aTarget"));

	ASSERT_EQ("aTarget_1", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_2"));

	ASSERT_FALSE(store.isDeltaRun("aTarget", "aTarget_1"));
	ASSERT_TRUE(store.isDeltaRun("aTarget", "aTarget_2"));

	store.addNewDeltaRun("aTarget", "aTarget_3", "aTarget_1");
	store.addNewDeltaRun("aTarget", "aTarget_4", "aTarget_1");
	store.addNewDeltaRun("aTarget", "aTarget_5", "aTarget_1");
	store.addNewDeltaRun("aTarget", "aTarget_6", "aTarget_1");

	store.addNewFullRun("aTarget", "aTarget_7");

	store.addNewDeltaRun("aTarget", "aTarget_8", "aTarget_7");
	store.addNewDeltaRun("aTarget", "aTarget_9", "aTarget_7");
	store.addNewDeltaRun("aTarget", "aTarget_10", "aTarget_7");

	ASSERT_EQ("aTarget_7", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_8"));
	ASSERT_EQ("aTarget_7", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_9"));
	ASSERT_EQ("aTarget_7", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_10"));

	ASSERT_EQ("aTarget_1", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_4"));
	ASSERT_EQ("aTarget_1", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_5"));
	ASSERT_EQ("aTarget_1", store.getCorrespondingFullRunForDelta("aTarget", "aTarget_6"));
}

TEST( StoreIndexTest, ReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	{
		JsonStoreIndex storer(tmp.path());

		storer.addNewFullRun("aTarget", "aTarget_1");
		storer.addNewDeltaRun("aTarget", "aTarget_2", "aTarget_1");


		storer.addNewDeltaRun("aTarget", "aTarget_3", "aTarget_1");
		storer.addNewDeltaRun("aTarget", "aTarget_4", "aTarget_1");
		storer.addNewDeltaRun("aTarget", "aTarget_5", "aTarget_1");
		storer.addNewDeltaRun("aTarget", "aTarget_6", "aTarget_1");

		storer.addNewFullRun("aTarget", "aTarget_7");

		storer.addNewDeltaRun("aTarget", "aTarget_8", "aTarget_7");
		storer.addNewDeltaRun("aTarget", "aTarget_9", "aTarget_7");
		storer.addNewDeltaRun("aTarget", "aTarget_10", "aTarget_7");
	}
	{
		JsonStoreIndex loader(tmp.path());

		ASSERT_EQ("aTarget_7", loader.getLastFullRun("aTarget"));

		ASSERT_EQ("aTarget_1", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_2"));

		ASSERT_FALSE(loader.isDeltaRun("aTarget", "aTarget_1"));
		ASSERT_TRUE(loader.isDeltaRun("aTarget", "aTarget_2"));

		ASSERT_EQ("aTarget_7", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_8"));
		ASSERT_EQ("aTarget_7", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_9"));
		ASSERT_EQ("aTarget_7", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_10"));

		ASSERT_EQ("aTarget_1", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_4"));
		ASSERT_EQ("aTarget_1", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_5"));
		ASSERT_EQ("aTarget_1", loader.getCorrespondingFullRunForDelta("aTarget", "aTarget_6"));
	}
}
