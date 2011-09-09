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

#include <string>
#include <gtest/gtest.h>
#include "Bacsy/Common/RunType.h"

using namespace Bacsy::Common;

using std::string;

TEST( RunTypeTests, SimpleTest )
{
	ASSERT_EQ("full", RunType::full.toString());
	ASSERT_EQ("fullfiles", RunType::fullfiles.toString());
	ASSERT_EQ("delta", RunType::delta.toString());

	ASSERT_EQ(RunType::full, RunType::fromString("full"));
	ASSERT_EQ(RunType::fullfiles, RunType::fromString("fullfiles"));
	ASSERT_EQ(RunType::delta, RunType::fromString("delta"));

	ASSERT_ANY_THROW(RunType::fromString("foo"));
}
