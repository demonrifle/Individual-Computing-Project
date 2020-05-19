// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/RandomStream.h"
#include "Math/Color.h"

#include "ProceduralPlanetMaterialSettings.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced)
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetMaterialSettings : public UObject
{
	GENERATED_BODY()
public :
		// Procedural Planet Material 
		UPROPERTY(EditAnywhere)
		UMaterialInterface* SphereMaterial;

	// Procedural Planet Texture1
	UPROPERTY(EditAnywhere)
		UTexture* Texture1;
	// Controls height for relating texture. 
	// 0 is ground level, 1 is max height after calculating highest layer point.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
		float Texture1Height;
	// Controls UV Tiling
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		FVector2D Texture1Tiling;

	// Procedural Planet Texture2
	UPROPERTY(EditAnywhere)
		UTexture* Texture2;
	// Controls height for relating texture. 
	// 0 is ground level, 1 is max height after calculating highest layer point.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
		float Texture2Height;
	// Controls UV Tiling
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		FVector2D Texture2Tiling;

	// Procedural Planet Texture3
	UPROPERTY(EditAnywhere)
		UTexture* Texture3;
	// Controls height for relating texture. 
	// 0 is ground level, 1 is max height after calculating highest layer point.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
		float Texture3Height;
	// Controls UV Tiling
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		FVector2D Texture3Tiling;

	// Procedural Planet Texture4
	UPROPERTY(EditAnywhere)
		UTexture* Texture4;
	// Controls height for relating texture. 
	// 0 is ground level, 1 is max height after calculating highest layer point.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
		float Texture4Height;
	// Controls UV Tiling
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		FVector2D Texture4Tiling;
private:
	UMaterial* PlanetMaterial;
	UMaterial* DefaultMaterial;
	UMaterialInstanceDynamic* MaterialInstance;
	UTexture* DefaultTexture;
public :
	UProceduralPlanetMaterialSettings();
	// Initialize all variables
	void Initialize();

	// Update all material instance parameters
	void Update();

	// Randomize values. 
	// Does not randomize textures.
	void Randomize(FRandomStream* Seed);

	// Returns the vertex coloring for a given 3D point based on percentage to top
	// Made for 4-texture height blending
	FColor GetVertexColorFor3DHeight(float Height, float MaxHeight);
	FColor GetVertexColorForHighestTexture();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
