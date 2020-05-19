// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetSettings.h"

UProceduralPlanetSettings::UProceduralPlanetSettings()
{

}

void UProceduralPlanetSettings::Initialize(bool IsRandom)
{
	MaterialSettings = NewObject<UProceduralPlanetMaterialSettings>(this);
	
	if (IsRandom)
	{
		MaterialSettings->Initialize();
		Seed = FRandomStream(FMath::Rand());
		Randomize();
	}
	else
	{	// Initialize the global seed for this planet
		Seed = FRandomStream(1337);

		// Set defaults
		Radius = 200.0f;
		Resolution = 50;
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

void UProceduralPlanetSettings::Randomize()
{		
	// Clamp seed to sensible values
	int32 NewSeed = Seed.RandRange(0, 100000);
	Seed = FRandomStream(NewSeed);

	Radius = Seed.FRandRange(200.0f, 500.0f);
	Resolution = Seed.RandRange(50, 200);

	MaterialSettings->Randomize(&Seed);

	// Add random layers of noise
	NoiseSettings.Empty();
	int NoiseLayers = Seed.RandRange(1, 3);
	for (int i = 0; i < NoiseLayers; i++)
	{
		NoiseSettings.Add(UNoiseLayer::GetRandomNoiseLayer(&Seed));
	}
}

void UProceduralPlanetSettings::RandomizeForSeed(int32 NewSeed)
{
	// Initialize the global seed for this planet
	Seed = FRandomStream(NewSeed);

	Radius = Seed.FRandRange(200.0f, 500.0f);
	Resolution = Seed.RandRange(50, 200);

	// Randomize material properties
	MaterialSettings->Randomize(&Seed);

	// Add random layers of noise
	NoiseSettings.Empty();
	int NoiseLayers = Seed.RandRange(1, 3);

	for (int i = 0; i < NoiseLayers; i++)
	{
		NoiseSettings.Add(UNoiseLayer::GetRandomNoiseLayer(&Seed));
	}
}

double UProceduralPlanetSettings::GetHeightAt3DPointForAllLayers(DVector Vector)
{
	double NoiseValue = 0.f;
	PerLayerTimer.Tick();
	for (UNoiseLayer* NoiseLayer : NoiseSettings)
	{
		if (NoiseLayer)
		{
			NoiseValue += NoiseLayer->GetHeightAt3DPoint(Vector);
		}
	}
	PerLayerTimer.Tock();
	return NoiseValue;
}

double UProceduralPlanetSettings::GetHeightAt3DPointMax()
{
	double MaxHeight = 0.0f;
	for (UNoiseLayer* NoiseLayer : NoiseSettings)
	{
		if (NoiseLayer)
		{
			MaxHeight += (NoiseLayer->Amplitude - NoiseLayer->ElevationReduction);
		}
	}
	return MaxHeight;
}

UMaterialInterface* UProceduralPlanetSettings::GetSphereMaterial()
{
	if (MaterialSettings)
	{
		return MaterialSettings->SphereMaterial;
	}
	return nullptr;
}

FColor UProceduralPlanetSettings::GetVertexColorFor3DHeight(float Height, float MaxHeight)
{
	if (MaterialSettings)
	{
		return MaterialSettings->GetVertexColorFor3DHeight(Height, MaxHeight);
	}
	return FColor(0, 0, 0, 0);
}

void UProceduralPlanetSettings::PrintLayerAverageSpeed()
{
	if (NoiseSettings.Num() != 0)
	{
		UNoiseLayer* NoiseLayer = NoiseSettings[0];
		if (NoiseLayer)
		{
			UE_LOG(ProceduralPlanetModule, Verbose, TEXT("Average speed for Layer : %.12dmilliseconds"), NoiseLayer->LayerTimer.Average);
		}
	}
}

#if WITH_EDITOR
void UProceduralPlanetSettings::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == "InitialSeed")
		{
			int32 NewSeed = Seed.GetInitialSeed();
			RandomizeForSeed(NewSeed);
		}

	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif