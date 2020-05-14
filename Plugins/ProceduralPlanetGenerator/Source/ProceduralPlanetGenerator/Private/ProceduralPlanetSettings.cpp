// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetSettings.h"

UProceduralPlanetSettings::UProceduralPlanetSettings()
{

}

void UProceduralPlanetSettings::Initialize(bool IsRandom)
{
	UMaterial* PlanetMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/ProceduralPlanetMaterial"));
	UMaterialInstanceDynamic* PlanetMaterialInstance = UMaterialInstanceDynamic::Create(PlanetMaterial, this);
	PlanetMaterialInstance->SetScalarParameterValue(FName("Texture2UTiling"), 5);
	PlanetMaterialInstance->SetScalarParameterValue(FName("Texture2UTiling"), 5);
	SphereMaterial = PlanetMaterialInstance;
	
	if (IsRandom)
	{
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

void UProceduralPlanetSettings::RandomizeForSeed(int32 NewSeed)
{
	// Initialize the global seed for this planet
	Seed = FRandomStream(NewSeed);

	Radius = Seed.FRandRange(200.0f, 500.0f);
	Resolution = Seed.RandRange(100, 300);

	// Add random layers of noise
	int NoiseLayers = Seed.RandRange(1, 3);

	for (int i = 0; i < NoiseLayers; i++)
	{
		NoiseSettings.Add(UNoiseLayer::GetRandomNoiseLayer(&Seed));
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

#if WITH_EDITOR
void UProceduralPlanetSettings::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == "InitialSeed")
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("seed changed"));

	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif