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

std::string format(const Poco::Timespan& timespan)
{
	return Poco::DateTimeFormatter::format(timespan, "%dd%Hh%Mm");
}

std::string getDelay(Poco::LocalDateTime now, std::string timeString)
{
	static TimerStringParser parser;
	return format((*(parser.parse(now, timeString).begin())).delay);
}

std::string getRepeat(Poco::LocalDateTime now, std::string timeString)
{
	static TimerStringParser parser;
	return format((*(parser.parse(now, timeString).begin())).repeat);
}

TEST( TimerStringParserTest, BasicWeeklyTest )
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

	ASSERT_EQ("7d00h00m", getRepeat(wednesday1400, "every week on Wednesday at 1500"));
	ASSERT_EQ("7d00h00m", getRepeat(wednesday1400, "every week on Friday at 0233"));
	ASSERT_EQ("7d00h00m", getRepeat(wednesday1400, "every week on Monday at 2005"));
	ASSERT_EQ("7d00h00m", getRepeat(wednesday1400, "every week on Tuesday at 1154"));
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

TEST( TimerStringParserTest, BasicDailyTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h05m", getDelay(wednesday1400, "every day at 1405h"));
	ASSERT_EQ("0d01h00m", getDelay(wednesday1400, "every day at 1500"));
	ASSERT_EQ("0d23h59m", getDelay(wednesday1400, "every day at 1359"));

	ASSERT_EQ("1d00h00m", getRepeat(wednesday1400, "every day at 1359"));
	ASSERT_EQ("1d00h00m", getRepeat(wednesday1400, "every day at 1500"));
}

TEST( TimerStringParserTest, BasicNMinutesTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 5 minutes"));
	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 100 minutes"));

	ASSERT_EQ("0d00h05m", getRepeat(wednesday1400, "every 5 minutes"));
	ASSERT_EQ("0d01h40m", getRepeat(wednesday1400, "every 100 minutes"));
}

TEST( TimerStringParserTest, BasicNHoursTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 5 hours"));
	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 2 hours"));

	ASSERT_EQ("0d05h00m", getRepeat(wednesday1400, "every 5 hours"));
	ASSERT_EQ("0d02h00m", getRepeat(wednesday1400, "every 2 hours"));
}

TEST( TimerStringParserTest, Every1MinuteTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 1 minute"));
	ASSERT_EQ("0d00h01m", getRepeat(wednesday1400, "every 1 minute"));
}

TEST( TimerStringParserTest, Every1HourTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "every 1 hour"));
	ASSERT_EQ("0d01h00m", getRepeat(wednesday1400, "every 1 hour"));
}

TEST( TimerStringParserTest, OnStartTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "on start"));
	ASSERT_EQ("0d00h00m", getRepeat(wednesday1400, "on start"));
}

TEST( TimerStringParserTest, AtStartTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);


	ASSERT_EQ("0d00h00m", getDelay(wednesday1400, "at start"));
	ASSERT_EQ("0d00h00m", getRepeat(wednesday1400, "at start"));
}


TEST (TimerStringParserTest, CompoundTest )
{
	Poco::LocalDateTime wednesday1400(
			2011,
			07,
			13,
			14,
			00);

	static TimerStringParser parser;
	std::list<TimeSchedule> rv = parser.parse(wednesday1400, "on start and every 10 minutes and every week on monday at 2000");

	ASSERT_EQ(3u, rv.size());
	std::list<TimeSchedule>::iterator it = rv.begin();

	ASSERT_EQ("0d00h00m", format((*it).delay));
	ASSERT_EQ("0d00h00m", format((*it).repeat));

	it++;

	ASSERT_EQ("0d00h00m", format((*it).delay));
	ASSERT_EQ("0d00h10m", format((*it).repeat));

	it++;

	ASSERT_EQ("5d06h00m", format((*it).delay));
	ASSERT_EQ("7d00h00m", format((*it).repeat));
}
