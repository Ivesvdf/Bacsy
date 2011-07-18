#ifndef TIMESCHEDULE_H
#define TIMESCHEDULE_H

#include "Poco/Timespan.h"

class TimeSchedule
{
public:
	TimeSchedule(const Poco::Timespan& delay, const Poco::Timespan& repeat):
		delay(delay), repeat(repeat)
	{}

	Poco::Timespan delay;
	Poco::Timespan repeat;
};

#endif
