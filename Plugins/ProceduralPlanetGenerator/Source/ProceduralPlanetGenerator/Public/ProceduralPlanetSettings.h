// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseLayer.h"
#include "Math/RandomStream.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralPlanetMaterialSettings.h"
#include "ProceduralPlanetGenerator.h"
#include "UtilityTimer.h"

#include "ProceduralPlanetSettings.generated.h"

// Plante settings class. Combines atomic and complex settings
UCLASS(DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetSettings : public UObject
{
	GENERATED_BODY()
public:
	// Planet seed. Use this to recreate the planet
	UPROPERTY(EditAnywhere, meta = (ClampMax = "100000"))
		FRandomStream Seed;
	// Planet radius. Negative values created inward-generated noise and a cave-like dome
	UPROPERTY(EditAnywhere, meta = (ClampMin = "-10000", ClampMax = "10000"))
		float Radius;
	// Planet density
	UPROPERTY(EditAnywhere, meta = (ClampMin = "32", ClampMax = "2048"))
		int32 Resolution;
	// Toggles realtime update of settings after any property changes
	UPROPERTY(EditAnywhere, meta = (DisplayName = "RealtimeUpdate"))
		bool EnableRealtimeUpdate;

	UtilityTimer PerLayerTimer;

private:
	UPROPERTY(EditAnywhere)
		UProceduralPlanetMaterialSettings* MaterialSettings;

	UPROPERTY(EditAnywhere, Instanced)
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

	// Retursn the sphere material
	UMaterialInterface* GetSphereMaterial();
	// Helper method to access the same function of the material settings
	FColor GetVertexColorFor3DHeight(float Height, float MaxHeight);

	void PrintLayerAverageSpeed();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
