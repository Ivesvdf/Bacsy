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
#include "Bacsy/Rules/ExclusionStringParser.h"

namespace Bacsy
{

typedef std::list<std::string> StringList;

TEST( ExclusionStringParserTests, OnlyLiteralTest)
{
	StringList one;
	one.push_back("C:/bla.txt");

	ASSERT_EQ(one, ExclusionStringParser::parse("C:/bla.txt"));
	ASSERT_EQ(one, ExclusionStringParser::parse("\"C:/bla.txt\""));

	StringList two;
	two.push_back("C:/bla txt");
	ASSERT_EQ(two, ExclusionStringParser::parse("C:/bla\\ txt"));

	StringList three;
	three.push_back("*.txt");
	three.push_back("*.dat");
	ASSERT_EQ(three, ExclusionStringParser::parse("*.txt *.dat"));

	StringList four;
	four.push_back("*.txt");
	four.push_back("*.dat");
	ASSERT_EQ(four, ExclusionStringParser::parse("*.txt \"*.dat\""));
}

TEST( ExclusionStringParserTests, EscapeQuoteInLiteral)
{
	StringList one;
	one.push_back("C:/bla\"txt");

	ASSERT_EQ(one, ExclusionStringParser::parse("\"C:/bla\\\"txt\""));
}

TEST( ExclusionStringParserTests, BackslashesAllowed)
{
	StringList one;
	one.push_back("C:\\bla.txt");

	ASSERT_EQ(one, ExclusionStringParser::parse("C:\\bla.txt"));
	ASSERT_EQ(one, ExclusionStringParser::parse("\"C:\\bla.txt\""));
}

TEST( ExclusionStringParserTests, AndTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExclusionStringParser::parse("*.txt & *.dat"));
}

TEST( ExclusionStringParserTests, SimpleNotTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("!");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExclusionStringParser::parse("*.txt & ! *.dat"));
}

TEST( ExclusionStringParserTests, MergedNotTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("!");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExclusionStringParser::parse("*.txt & !*.dat"));
}

TEST( ExclusionStringParserTests, SemiRealisticExample)
{
	StringList one;
	one.push_back("/home/ives/.*");
	one.push_back("&");
	one.push_back("!");
	one.push_back(".bashrc");
	one.push_back("&");
	one.push_back("!");
	one.push_back(".vimrc");
	one.push_back("&");
	one.push_back("<1MB");

	ASSERT_EQ(one, ExclusionStringParser::parse("/home/ives/.* & !.bashrc & !.vimrc & <1MB"));
}


TEST( ExclusionStringParserTests, VerySpecialQuotedCase)
{
	// For people who want \" in their filenames
	StringList one;
	one.push_back("bla\\\"bla");

	ASSERT_EQ(one, ExclusionStringParser::parse("\"bla\\\\\\\"bla\""));
}


TEST( ExclusionStringParserTests, BackslashAtTheEnd)
{
	StringList one;
	one.push_back("C:\\dir\\");

	ASSERT_EQ(one, ExclusionStringParser::parse("\"C:\\dir\\\\\""));
}

}
