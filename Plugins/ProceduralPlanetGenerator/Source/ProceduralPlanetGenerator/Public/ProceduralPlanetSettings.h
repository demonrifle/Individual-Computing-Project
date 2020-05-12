// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseLayer.h"
#include "Math/RandomStream.h"

#include "ProceduralPlanetSettings.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "GeneralPlanetSettings")
		FRandomStream Seed;
	UPROPERTY(Category = "GeneralPlanetSettings", EditAnywhere)
		float Radius;
	UPROPERTY(Category = "GeneralPlanetSettings", EditAnywhere)
		int32 Resolution;
	
	UPROPERTY(EditAnywhere, Instanced)
		TArray<UNoiseLayer*> NoiseSettings;
public:
	UProceduralPlanetSettings();
	
	void Initialize(bool IsRandom);
	void UpdateNoiseSettings();
	double GetHeightAt3DPointForAllLayers(DVector Vector);
};
