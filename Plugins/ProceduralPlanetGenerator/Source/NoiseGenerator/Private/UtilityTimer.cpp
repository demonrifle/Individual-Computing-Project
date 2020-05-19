// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "UtilityTimer.h"
#include "GenericPlatform/GenericPlatformTime.h"


UtilityTimer::UtilityTimer()
{
	TickTime = 0;
	TockTime = 0;
}

UtilityTimer::~UtilityTimer()
{
}


void UtilityTimer::Tick()
{
	TickTime = FPlatformTime::ToMilliseconds64(FPlatformTime::Cycles64());

}

double UtilityTimer::Tock()
{
	TockTime = FPlatformTime::ToMilliseconds64(FPlatformTime::Cycles64());
	RecalculateAverage(TockTime-TickTime);
	return TockTime - TickTime;
}

double UtilityTimer::RecalculateAverage(double Addition)
{
	Total += Addition;
	Number++;
	Average = Total / Number;
	return Average;
}