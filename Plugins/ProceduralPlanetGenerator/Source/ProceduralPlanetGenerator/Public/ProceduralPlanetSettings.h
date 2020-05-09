// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseLayer.h"

#include "ProceduralPlanetSettings.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "GeneralPlanetSettings", EditAnywhere)
		float Radius;
	UPROPERTY(Category = "GeneralPlanetSettings", EditAnywhere)
		int32 Resolution;
	
	UPROPERTY(EditAnywhere, Instanced)
		TArray<UNoiseLayer*> NoiseSettings;
public:
	UProceduralPlanetSettings();
};
