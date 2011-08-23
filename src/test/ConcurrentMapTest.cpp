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
#include "ConcurrentMap.h"

using std::string;

TEST( ConcurrentMapTest, SimpleTest )
{
	ConcurrentMap<int, string> stringMap;

	ASSERT_EQ(0u, stringMap.count(5));
	ASSERT_EQ("", stringMap.get(5));

	ASSERT_EQ(0u, stringMap.count(6));
	stringMap.set(6, "hello");
	ASSERT_EQ(1u, stringMap.count(6));
	ASSERT_EQ("hello", stringMap.get(6));

	stringMap.erase(6);
	ASSERT_EQ(0u, stringMap.count(6));
	ASSERT_EQ("", stringMap.get(6));
	ASSERT_EQ(1u, stringMap.count(6));
}

