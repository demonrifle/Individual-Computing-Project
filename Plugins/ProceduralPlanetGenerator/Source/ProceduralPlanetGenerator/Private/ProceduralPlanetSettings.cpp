// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetSettings.h"

UProceduralPlanetSettings::UProceduralPlanetSettings()
{
	Radius = 200.0f;
	Resolution = 50;
	NoiseSettings.Add(CreateDefaultSubobject<UNoiseLayer>(TEXT("Noise Layers")));
}