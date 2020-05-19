// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NOISEGENERATOR_API UtilityTimer
{
public:

	double TickTime;
	double TockTime;
	double Total;
	int Number;
	double Average;


public:
	UtilityTimer();
	~UtilityTimer();

	void Tick();
	double Tock();
	double RecalculateAverage(double Addition);
};
