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

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Bacsy/Server/StoreTimeParser.h"

using namespace Bacsy::Server;

TEST( StoreTimeParserTest, Basic1day )
{
	ASSERT_EQ(24*60*60u, StoreTimeParser::parse("1 day"));
	ASSERT_EQ(24*60*60u, StoreTimeParser::parse("1 days"));
}

TEST( StoreTimeParserTest, Basic2days )
{
	ASSERT_EQ(2*24*60*60u, StoreTimeParser::parse("2 day"));
	ASSERT_EQ(2*24*60*60u, StoreTimeParser::parse("2 days"));
}

TEST( StoreTimeParserTest, Basic8months )
{
	ASSERT_EQ(8*31*24*60*60u, StoreTimeParser::parse("8 month"));
	ASSERT_EQ(8*31*24*60*60u, StoreTimeParser::parse("8 months"));
}

TEST( StoreTimeParserTest, Basic42years )
{
	ASSERT_EQ(42*366*24*60*60u, StoreTimeParser::parse("42 year"));
	ASSERT_EQ(42*366*24*60*60u, StoreTimeParser::parse("42 years"));
}
