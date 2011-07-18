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

#include <utility>
#include "Poco/RegularExpression.h"
#include "Poco/Timespan.h"
#include "woodcutter/woodcutter.h"
#include "stringUtils.h"
#include "timerStringParser.h"


TimerStringParser::TimerStringParser():
	timeRegex("[0-9]{2}(:|h)?[0-9]{2}h?")
{

}

int TimerStringParser::extractDay(const std::string& input)
{
	Poco::RegularExpression getWeekday("[a-z]+day");
	std::string dayString;
	getWeekday.extract(input, dayString);

	if(dayString == "sunday")
		return 0;
	else if(dayString == "monday")
		return 1;
	else if(dayString == "tuesday")
		return 2;
	else if(dayString == "wednesday")
		return 3;
	else if(dayString == "thursday")
		return 4;
	else if(dayString == "friday")
		return 5;
	else if(dayString == "saturday")
		return 6;
	else
		return -1;

}

std::string TimerStringParser::extractTime(const std::string& input)
{
	std::string time;
	const Poco::RegularExpression timeExtractor(timeRegex);
	timeExtractor.extract(input, time);
	return time;
}

std::pair<int, int> parseTime(const std::string& time)
{
	const int hour = StringUtils::fromString<int>(time.substr(0,2));

	int firstMinPos = 2;

	if(time[firstMinPos] == ':' || time[firstMinPos] == 'h')
		firstMinPos++;

	const int minutes = StringUtils::fromString<int>(time.substr(firstMinPos, 2));

	return std::make_pair(hour, minutes);
}

std::list<TimeSchedule> TimerStringParser::parse(Poco::LocalDateTime now, std::string timerString)
{
	timerString = StringUtils::toLower(timerString);
	Poco::RegularExpression isWeekly("every week on [a-z]+day at " + timeRegex);
	Poco::RegularExpression isDaily("every day at " + timeRegex);

	if(isWeekly.match(timerString))
	{
		return parseWeekly(now, timerString);
	}
	if(isDaily.match(timerString))
	{
		return parseDaily(now, timerString);
	}
	else
	{
		LOGE("No match found for timerString " + timerString);
	}

	// No match... no timers.
	return std::list<TimeSchedule>();
}

std::list<TimeSchedule> TimerStringParser::parseDaily(const Poco::LocalDateTime& now, const std::string& timerString)
{
	std::pair<int, int> hourMinutes = parseTime(extractTime(timerString));
	const int hour = hourMinutes.first;
	const int minutes = hourMinutes.second;

	int dayOffset = 0;
	Poco::LocalDateTime ifToday(
			now.year(),
			now.month(),
			now.day(),
			hour,
			minutes);

	if(ifToday < now)
	{
		LOGI("It's to late to run " + timerString + " today; running next week.");
		dayOffset = 1;
	}

	Poco::LocalDateTime tmp(
			now.year(),
			now.month(),
			now.day(),
			hour,
			minutes);
	
	const Poco::Timespan daysToWaitTimespan(dayOffset*Poco::Timespan::DAYS);
	const Poco::LocalDateTime target = tmp + daysToWaitTimespan;

	const Poco::Timespan wait = target - now;

	std::list<TimeSchedule> rv;
	rv.push_back(TimeSchedule(wait, Poco::Timespan(1*Poco::Timespan::DAYS)));
	return rv;
}


std::list<TimeSchedule> TimerStringParser::parseWeekly(const Poco::LocalDateTime& now, const std::string& timerString)
{
	const int nowDay = now.dayOfWeek(); // 0 = Sunday, 6 = Saturday
	int targetDay = extractDay(timerString); // 0 = Sunday, 6 = Saturday

	if(targetDay < 0)
	{
		LOGE("Invalid day of week in timerString " + timerString);
	}

	if(targetDay < nowDay) // wait until next week...
		targetDay += 7;

	int daysToWait = targetDay - nowDay;

	std::pair<int, int> hourMinutes = parseTime(extractTime(timerString));
	const int hour = hourMinutes.first;
	const int minutes = hourMinutes.second;

	// If the timer runs today, check if it already ran or if it will
	// still run today
	if(daysToWait == 0)
	{
		Poco::LocalDateTime ifToday(
				now.year(),
				now.month(),
				now.day(),
				hour,
				minutes);

		if(ifToday < now)
		{
			LOGI("It's to late to run " + timerString + " today; running next week.");
			daysToWait = 7;
		}
	}

	// At this point, I'd just like to add daysToWait to now and combine
	// this with hour and minutes to know when to run the next time.
	// Unfortunately, unless I'm missing something, this isn't that easy.
	// Since the Poco LocalDateTime and DateTime constructors refuse to do
	// date arithmetic (e.g. day 35 will be somewhere in the next month),
	// I'll have to construct a Timespan that spans the correct number of
	// days, add that to now and then use the day, month & year info in
	// the resulting date. Which can then be combined with hour & minute
	// to find when the next run will be. 
	Poco::Timespan daysToWaitTimespan(daysToWait*Poco::Timespan::DAYS);

	Poco::LocalDateTime correctDay = now + daysToWaitTimespan;
	Poco::LocalDateTime correctEverything(
			correctDay.year(),
			correctDay.month(),
			correctDay.day(),
			hour,
			minutes);

	Poco::Timespan wait = correctEverything - now;

	std::list<TimeSchedule> rv;
	rv.push_back(TimeSchedule(wait, Poco::Timespan(7*Poco::Timespan::DAYS)));
	return rv;
}
