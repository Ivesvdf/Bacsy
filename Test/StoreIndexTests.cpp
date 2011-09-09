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
#include "Poco/Timestamp.h"
#include "Bacsy/Server/JsonStoreIndex.h"

using namespace Bacsy::Server;

TEST( StoreIndexTests, NoReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	JsonStoreIndex store(tmp.path());
	
	ASSERT_EQ("", store.getLastFullRun("IX", "aSource"));
	store.addNewFullRun("IX", "aSource", "aSource_1", Poco::Timestamp());
	ASSERT_EQ("aSource_1", store.getLastFullRun("IX", "aSource"));
	
	store.addNewDeltaRun("IX", "aSource", "aSource_2", "aSource_1", Poco::Timestamp());
	ASSERT_EQ("aSource_1", store.getLastFullRun("IX", "aSource"));

	ASSERT_EQ("aSource_1", store.getCorrespondingFullRun("IX", "aSource", "aSource_2"));

	ASSERT_FALSE(store.isDeltaRun("IX", "aSource", "aSource_1"));
	ASSERT_TRUE(store.isDeltaRun("IX", "aSource", "aSource_2"));

	store.addNewDeltaRun("IX", "aSource", "aSource_3", "aSource_1", Poco::Timestamp());
	store.addNewDeltaRun("IX", "aSource", "aSource_4", "aSource_1", Poco::Timestamp());
	store.addNewDeltaRun("IX", "aSource", "aSource_5", "aSource_1", Poco::Timestamp());
	store.addNewDeltaRun("IX", "aSource", "aSource_6", "aSource_1", Poco::Timestamp());

	store.addNewFullRun("IX", "aSource", "aSource_7", Poco::Timestamp());

	store.addNewFullFilesRun("IX", "aSource", "aSource_8", "aSource_7", Poco::Timestamp());
	store.addNewFullFilesRun("IX", "aSource", "aSource_9", "aSource_7", Poco::Timestamp());
	store.addNewFullFilesRun("IX", "aSource", "aSource_10", "aSource_7", Poco::Timestamp());

	ASSERT_EQ("aSource_7", store.getCorrespondingFullRun("IX", "aSource", "aSource_8"));
	ASSERT_EQ("aSource_7", store.getCorrespondingFullRun("IX", "aSource", "aSource_9"));
	ASSERT_EQ("aSource_7", store.getCorrespondingFullRun("IX", "aSource", "aSource_10"));

	ASSERT_EQ("aSource_1", store.getCorrespondingFullRun("IX", "aSource", "aSource_4"));
	ASSERT_EQ("aSource_1", store.getCorrespondingFullRun("IX", "aSource", "aSource_5"));
	ASSERT_EQ("aSource_1", store.getCorrespondingFullRun("IX", "aSource", "aSource_6"));
}

TEST( StoreIndexTests, ReloadJsonTest )
{
	Poco::TemporaryFile tmp;

	{
		JsonStoreIndex storer(tmp.path());

		storer.addNewFullRun("IX", "aSource", "aSource_1", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_2", "aSource_1", Poco::Timestamp());


		storer.addNewDeltaRun("IX", "aSource", "aSource_3", "aSource_1", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_4", "aSource_1", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_5", "aSource_1", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_6", "aSource_1", Poco::Timestamp());

		storer.addNewFullRun("IX", "aSource", "aSource_7", Poco::Timestamp());

		storer.addNewDeltaRun("IX", "aSource", "aSource_8", "aSource_7", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_9", "aSource_7", Poco::Timestamp());
		storer.addNewDeltaRun("IX", "aSource", "aSource_10", "aSource_7", Poco::Timestamp());
	}
	{
		JsonStoreIndex loader(tmp.path());

		ASSERT_EQ("aSource_7", loader.getLastFullRun("IX", "aSource"));

		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRun("IX", "aSource", "aSource_2"));

		ASSERT_FALSE(loader.isDeltaRun("IX", "aSource", "aSource_1"));
		ASSERT_TRUE(loader.isDeltaRun("IX", "aSource", "aSource_2"));

		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRun("IX", "aSource", "aSource_8"));
		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRun("IX", "aSource", "aSource_9"));
		ASSERT_EQ("aSource_7", loader.getCorrespondingFullRun("IX", "aSource", "aSource_10"));

		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRun("IX", "aSource", "aSource_4"));
		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRun("IX", "aSource", "aSource_5"));
		ASSERT_EQ("aSource_1", loader.getCorrespondingFullRun("IX", "aSource", "aSource_6"));
	}
}
