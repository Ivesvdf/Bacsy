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

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "timerStringParser.h"

std::string getDelay(Poco::LocalDateTime now, std::string timeString)
{
	static TimerStringParser parser;
	return Poco::DateTimeFormatter::format((*(parser.parse(now, timeString).begin())).delay, "%dd%Hh%Mm");
}

TEST( TimerStringParserTest, TodayTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("6d23h20m", getDelay(wednesday1400, "every week on Wednesday at 1320"));
	ASSERT_EQ("6d23h29m", getDelay(wednesday1400, "every week on Wednesday at 1329"));
	ASSERT_EQ("0d01h00m", getDelay(wednesday1400, "every week on Wednesday at 1500"));
	ASSERT_EQ("1d06h00m", getDelay(wednesday1400, "every week on Thursday at 20h00"));
	ASSERT_EQ("2d06h00m", getDelay(wednesday1400, "every week on Friday at 20h00"));
	ASSERT_EQ("3d06h00m", getDelay(wednesday1400, "every week on Saturday at 20h00"));
	ASSERT_EQ("4d06h00m", getDelay(wednesday1400, "every week on Sunday at 20h00"));
	ASSERT_EQ("5d06h00m", getDelay(wednesday1400, "every week on Monday at 20h00"));
	ASSERT_EQ("6d06h00m", getDelay(wednesday1400, "every week on Tuesday at 20h00"));
}

TEST( TimerStringParserTest, SaturdayToSunday )
{
	Poco::LocalDateTime saturday1500(
			2011,
			07,
			16,
			15,
			00);


	ASSERT_EQ("1d00h00m", getDelay(saturday1500, "every week on Sunday at 1500"));
	ASSERT_EQ("0d23h00m", getDelay(saturday1500, "every week on Sunday at 1400"));
}

TEST( TimerStringParserTest, SundayToSaturday )
{
	Poco::LocalDateTime sunday1500(
			2011,
			07,
			17,
			15,
			00);


	ASSERT_EQ("6d00h00m", getDelay(sunday1500, "every week on Saturday at 1500"));
	ASSERT_EQ("5d23h00m", getDelay(sunday1500, "every week on Saturday at 1400"));
}

TEST( TimerStringParserTest, TestTimeStrings )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("6d23h20m", getDelay(wednesday1400, "every week on Wednesday at 1320"));
	ASSERT_EQ("6d23h20m", getDelay(wednesday1400, "every week on Wednesday at 1320h"));
	ASSERT_EQ("6d23h20m", getDelay(wednesday1400, "every week on Wednesday at 13:20"));
	ASSERT_EQ("6d23h20m", getDelay(wednesday1400, "every week on Wednesday at 13:20h"));
}
