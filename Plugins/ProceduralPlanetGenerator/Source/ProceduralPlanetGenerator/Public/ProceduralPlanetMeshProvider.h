// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "ProceduralPlanetSettings.h"
#include "NoiseLayer.h"

#include "ProceduralPlanetMeshProvider.generated.h"


// This class extends the RMC provider behaviour to build the mesh for a procedural planet
// Most of this is copied from the RuntimeMeshProviderSphere with added functionality for noise
// TODO : At a later stage should be refactored to only require a ProceduralPlanetSettings object/pointer since radius/segments etc can already be found there
UCLASS()
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetMeshProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	mutable FCriticalSection PropertySyncRoot;

		int32 MaxLOD;

		float SphereRadius;

		int32 MaxSegments;
		int32 MinSegments;


		float LODMultiplier;

		UMaterialInterface* SphereMaterial;

		UProceduralPlanetSettings* ProceduralPlanetSettings;

public:

	UProceduralPlanetMeshProvider();

		float GetSphereRadius() const;
		void SetSphereRadius(float InSphereRadius);

		int32 GetMaxSegments() const;
		void SetMaxSegments(int32 InMaxSegments);

		int32 GetMinSegments() const;
		void SetMinSegments(int32 InMinSegments);

		float GetLODMultiplier() const;
		void SetLODMultiplier(float InLODMultiplier);

		UMaterialInterface* GetSphereMaterial() const;
		void SetSphereMaterial(UMaterialInterface* InSphereMaterial);

		UProceduralPlanetSettings* GetProceduralPlanetSettings() const;
		void SetProceduralPlanetSettings(UProceduralPlanetSettings* InProceduralPlanetSettings);

protected:
	virtual void Initialize_Implementation() override;
	virtual bool GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	virtual FRuntimeMeshCollisionSettings GetCollisionSettings_Implementation() override;
	virtual FBoxSphereBounds GetBounds_Implementation() override;
	virtual bool IsThreadSafe_Implementation() override;


private:
	void GetShapeParams(float& OutRadius, int32& OutMinSegments, int32& OutMaxSegments, float& OutLODMultiplier);
	int32 GetMaxNumberOfLODs();
	float CalculateScreenSize(int32 LODIndex);
	void GetSegmentsForLOD(int32& LODIndex, int32& Segments)
	{
		GetSegmentsForLOD(LODIndex, LODMultiplier, MaxSegments, MinSegments, Segments);
	}
	static void GetSegmentsForLOD(int32& LODIndex, float& LODMul, int32& MaxSegments, int32& MinSegments,  int32& Segments) {
		Segments = FMath::Max(FMath::RoundToInt(MaxSegments * FMath::Pow(LODMul, LODIndex)), MinSegments);
	}
	static bool GetSphereMesh(int32 SphereRadius, int32 Segments, FRuntimeMeshRenderableMeshData& MeshData, UProceduralPlanetSettings* PlanetSettings);
	void UpdateMeshParameters(bool bAffectsCollision);

};
