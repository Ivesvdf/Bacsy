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
#include "Bacsy/Common/StringUtils.h"

using namespace Bacsy::Common;

TEST( StringUtilsTest, SimpleStripTest )
{
	ASSERT_EQ("x", StringUtils::rstrip("xyyy", "y"));
	ASSERT_EQ("xyyy", StringUtils::lstrip("xyyy", "y"));
	ASSERT_EQ("x", StringUtils::strip("xyyy", "y"));
	ASSERT_EQ("bcde", StringUtils::strip("abcdea", "a"));
}

TEST( StringUtilsTest, MultipleStripDropTest ) 
{
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ab"));
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ba"));
}

TEST( StringUtilsTest, DefaultStripTest )
{
	ASSERT_EQ("text ", StringUtils::lstrip("  text "));
	ASSERT_EQ("  text", StringUtils::rstrip("  text "));
	ASSERT_EQ("text", StringUtils::strip("text"));
}

TEST( StringUtilsTest, SplitTest )
{
	typedef std::vector<std::string> VS;

	VS a = StringUtils::split("a", ',');
	ASSERT_EQ("a", a[0]); 
	ASSERT_EQ(1u, a.size());

	VS b = StringUtils::split("a,b", ',');
	ASSERT_EQ("a", b[0]); 
	ASSERT_EQ("b", b[1]); 
	ASSERT_EQ(2u, b.size());

	VS c = StringUtils::split("a,b", ' ');
	ASSERT_EQ("a,b", c[0]); 
	ASSERT_EQ(1u, c.size());

	VS d = StringUtils::split("Is this real life?", ' ');
	ASSERT_EQ("Is", d[0]); 
	ASSERT_EQ("this", d[1]); 
	ASSERT_EQ("real", d[2]); 
	ASSERT_EQ("life?", d[3]); 
	ASSERT_EQ(4u, d.size());
}

TEST( StringUtilsTest, StringSplitTest )
{
	typedef std::vector<std::string> VS;

	VS a = StringUtils::split("abc", "b");
	ASSERT_EQ(2u, a.size());
	ASSERT_EQ("a", a[0]); 
	ASSERT_EQ("c", a[1]); 

	VS b = StringUtils::split("abc", "d");
	ASSERT_EQ(1u, b.size());
	ASSERT_EQ("abc", b[0]); 
	
	VS c = StringUtils::split("me and myself and I", " and ");
	ASSERT_EQ(3u, c.size());
	ASSERT_EQ("me", c[0]); 
	ASSERT_EQ("myself", c[1]); 
	ASSERT_EQ("I", c[2]); 
}


TEST( StringUtilsTest, ToLowerTest)
{
	ASSERT_EQ("bla", StringUtils::toLower("bla"));
	ASSERT_EQ("bla", StringUtils::toLower("Bla"));
	ASSERT_EQ("bla", StringUtils::toLower("blA"));
	ASSERT_EQ("bla", StringUtils::toLower("BLA"));
	ASSERT_EQ("one two three", StringUtils::toLower("one Two three"));
}

TEST(StringUtilsTest, IsTextTest)
{
	ASSERT_EQ(true, StringUtils::isLetters("foo"));
	ASSERT_EQ(false, StringUtils::isLetters("foo bar"));
	ASSERT_EQ(false, StringUtils::isLetters("foo9"));
	ASSERT_EQ(false, StringUtils::isLetters("9"));
}

TEST(StringUtilsTest, IsNumberTest)
{
	ASSERT_EQ(true, StringUtils::isInteger("5"));
	ASSERT_EQ(true, StringUtils::isInteger("1993"));
	ASSERT_EQ(false, StringUtils::isInteger("foo9"));
	ASSERT_EQ(false, StringUtils::isInteger("15foo"));
	ASSERT_EQ(false, StringUtils::isInteger("fo1o"));
}
