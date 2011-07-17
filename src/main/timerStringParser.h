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

#ifndef TIMERSTRINGPARSER_H
#define TIMERSTRINGPARSER_H

#include <string>
#include "Poco/LocalDateTime.h"

class TimerStringParser
{
private:
	const std::string timeRegex;
public:
	TimerStringParser();
	void parse(Poco::LocalDateTime now, std::string timerString);

private:
	void parseWeekly(const Poco::LocalDateTime& now, const std::string& timerString);
	void parseDaily(const Poco::LocalDateTime& now, const std::string& timerString);
	int extractDay(const std::string& input);
	std::string extractTime(const std::string& input);
	
	

};

#endif
