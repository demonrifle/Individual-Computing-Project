// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseLayer.h"
#include "Math/RandomStream.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralPlanetMaterialSettings.h"

#include "ProceduralPlanetSettings.generated.h"

// Plante settings class. Combines atomic and complex settings
UCLASS(EditInlineNew, DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetSettings : public UActorComponent
{
	GENERATED_BODY()
public:
	// Planet seed. Use this to recreate the planet
	UPROPERTY(EditAnywhere, Category = "Planet Settings")
		FRandomStream Seed;
	// Planet radius. 
	UPROPERTY(EditAnywhere, Category = "Planet Settings")
		float Radius;
	// Planet density
	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (ClampMin = "32", ClampMax = "2048"))
		int32 Resolution;

	UPROPERTY(EditAnywhere, Instanced, Category = "Material Settings")
		UProceduralPlanetMaterialSettings* MaterialSettings;

	UPROPERTY(EditAnywhere, Instanced, Category = "Noise Settings")
		TArray<UNoiseLayer*> NoiseSettings;
public:
	UProceduralPlanetSettings();
	
	void Initialize(bool IsRandom);

	// Calls update settings inside the FastNoise class.
	void UpdateNoiseSettings();

	void Randomize();
	void RandomizeForSeed(int32 NewSeed);

	// Easy getter method which calculates applied noise of all layers
	double GetHeightAt3DPointForAllLayers(DVector Vector);
	// Returns the maximum possible height of the current noise settings
	double GetHeightAt3DPointMax();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
