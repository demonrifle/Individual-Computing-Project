// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseLayer.h"
#include "Math/RandomStream.h"

#include "ProceduralPlanetSettings.generated.h"

// Plante settings class. Combines atomic and complex settings
UCLASS(DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetSettings : public UActorComponent
{
	GENERATED_BODY()
public:
	// Planet seed. Use this to recreate the planet
	UPROPERTY(EditAnywhere)
		FRandomStream Seed;
	// Planet radius. 
	UPROPERTY(EditAnywhere)
		float Radius;
	// Planet density
	UPROPERTY(EditAnywhere, meta = (ClampMin = "32", ClampMax = "2048"))
		int32 Resolution;

	// Material properties 
	UPROPERTY(EditAnywhere)
	UMaterialInterface* SphereMaterial;
	
	UPROPERTY(EditAnywhere, Instanced)
		TArray<UNoiseLayer*> NoiseSettings;
public:
	UProceduralPlanetSettings();
	
	void Initialize(bool IsRandom);

	// Calls update settings inside the FastNoise class.
	void UpdateNoiseSettings();

	// Easy getter method which calculates applied noise of all layers
	double GetHeightAt3DPointForAllLayers(DVector Vector);
	// Returns the maximum possible height of the current noise settings
	double GetHeightAt3DPointMax();
};
