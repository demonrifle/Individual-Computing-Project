// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetSettings.h"

UProceduralPlanetSettings::UProceduralPlanetSettings()
{
	Initialize(false);
}

void UProceduralPlanetSettings::Initialize(bool IsRandom)
{

	if (IsRandom)
	{
		// Initialize the global seed for this planet
		Seed = FRandomStream();
		Seed.GenerateNewSeed();

		Radius = Seed.FRandRange(200.0f, 500.0f);
		Resolution = Seed.RandRange(100, 300);

		// Add random layers of noise
		int NoiseLayers = Seed.RandRange(1, 3);

		for (int i = 0; i < NoiseLayers; i++)
		{
			NoiseSettings.Add(UNoiseLayer::GetRandomNoiseLayer(&Seed));
		}

	}
	else
	{	// Initialize the global seed for this planet
		Seed = FRandomStream(1337);

		// Set defaults
		Radius = 200.0f;
		Resolution = 50;

		// One default uninitialized noise layer
		NoiseSettings.Add(NewObject<UNoiseLayer>(this, TEXT("Noise Layers")));
	}
}

void UProceduralPlanetSettings::UpdateNoiseSettings()
{
	for (UNoiseLayer* NoiseLayer : NoiseSettings)
	{
		if (NoiseLayer)
		{
			NoiseLayer->UpdateValues();
		}
	}
}

double UProceduralPlanetSettings::GetHeightAt3DPointForAllLayers(DVector Vector)
{
	double NoiseValue = 0.f;
	for (UNoiseLayer* NoiseLayer : NoiseSettings)
	{
		if (NoiseLayer)
		{
			NoiseValue += NoiseLayer->GetHeightAt3DPoint(Vector);
		}
	}
	return NoiseValue;
}
