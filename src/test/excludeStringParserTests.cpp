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
#include "excludeStringParser.h"

typedef std::list<std::string> StringList;

TEST( ExcludeStringParserTest, OnlyLiteralTest)
{
	StringList one;
	one.push_back("C:/bla.txt");

	ASSERT_EQ(one, ExcludeStringParser::parse("C:/bla.txt"));
	ASSERT_EQ(one, ExcludeStringParser::parse("\"C:/bla.txt\""));

	StringList two;
	two.push_back("C:/bla txt");
	ASSERT_EQ(two, ExcludeStringParser::parse("C:/bla\\ txt"));

	StringList three;
	three.push_back("*.txt");
	three.push_back("*.dat");
	ASSERT_EQ(three, ExcludeStringParser::parse("*.txt *.dat"));

	StringList four;
	four.push_back("*.txt");
	four.push_back("*.dat");
	ASSERT_EQ(four, ExcludeStringParser::parse("*.txt \"*.dat\""));
}

TEST( ExcludeStringParserTest, EscapeQuoteInLiteral)
{
	StringList one;
	one.push_back("C:/bla\"txt");

	ASSERT_EQ(one, ExcludeStringParser::parse("\"C:/bla\\\"txt\""));
}

TEST( ExcludeStringParserTest, BackslashesAllowed)
{
	StringList one;
	one.push_back("C:\\bla.txt");

	ASSERT_EQ(one, ExcludeStringParser::parse("C:\\bla.txt"));
	ASSERT_EQ(one, ExcludeStringParser::parse("\"C:\\bla.txt\""));
}

TEST( ExcludeStringParserTest, AndTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExcludeStringParser::parse("*.txt & *.dat"));
}

TEST( ExcludeStringParserTest, SimpleNotTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("!");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExcludeStringParser::parse("*.txt & ! *.dat"));
}

TEST( ExcludeStringParserTest, MergedNotTest)
{
	StringList one;
	one.push_back("*.txt");
	one.push_back("&");
	one.push_back("!");
	one.push_back("*.dat");

	ASSERT_EQ(one, ExcludeStringParser::parse("*.txt & !*.dat"));
}

TEST( ExcludeStringParserTest, SemiRealisticExample)
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

	ASSERT_EQ(one, ExcludeStringParser::parse("/home/ives/.* & !.bashrc & !.vimrc & <1MB"));
}
