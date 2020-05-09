// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "NoiseLayer.h"

#include "ProceduralPlanetMeshProvider.generated.h"


UCLASS()
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetMeshProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	mutable FCriticalSection PropertySyncRoot;

		int32 MaxLOD;

		float SphereRadius;

		int32 MaxLatitudeSegments;
		int32 MinLatitudeSegments;

		int32 MaxLongitudeSegments;
		int32 MinLongitudeSegments;

		float LODMultiplier;

		UMaterialInterface* SphereMaterial;

		TArray<UNoiseLayer*> Noise;

public:

	UProceduralPlanetMeshProvider();

		float GetSphereRadius() const;
		void SetSphereRadius(float InSphereRadius);

		int32 GetMaxLatitudeSegments() const;
		void SetMaxLatitudeSegments(int32 InMaxLatitudeSegments);

		int32 GetMinLatitudeSegments() const;
		void SetMinLatitudeSegments(int32 InMinLatitudeSegments);

		int32 GetMaxLongitudeSegments() const;
		void SetMaxLongitudeSegments(int32 InMaxLongitudeSegments);

		int32 GetMinLongitudeSegments() const;
		void SetMinLongitudeSegments(int32 InMinLongitudeSegments);

		float GetLODMultiplier() const;
		void SetLODMultiplier(float InLODMultiplier);

		UMaterialInterface* GetSphereMaterial() const;
		void SetSphereMaterial(UMaterialInterface* InSphereMaterial);

		TArray<UNoiseLayer*> GetNoise() const;
		void SetNoise(TArray<UNoiseLayer*> InNoiseLayer);

protected:
	virtual void Initialize_Implementation() override;
	virtual bool GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	virtual FRuntimeMeshCollisionSettings GetCollisionSettings_Implementation() override;
	virtual FBoxSphereBounds GetBounds_Implementation() override;
	virtual bool IsThreadSafe_Implementation() override;


private:
	void GetShapeParams(float& OutRadius, int32& OutMinLatitudeSegments, int32& OutMaxLatitudeSegments, int32& OutMinLongitudeSegments, int32& OutMaxLongitudeSegments, float& OutLODMultiplier);
	int32 GetMaxNumberOfLODs();
	float CalculateScreenSize(int32 LODIndex);
	void GetSegmentsForLOD(int32& LODIndex, int32& LatitudeSegments, int32& LongitudeSegments)
	{
		GetSegmentsForLOD(LODIndex, LODMultiplier, MaxLatitudeSegments, MinLatitudeSegments, MaxLongitudeSegments, MinLongitudeSegments, LatitudeSegments, LongitudeSegments);
	}
	static void GetSegmentsForLOD(int32& LODIndex, float& LODMul, int32& MaxLat, int32& MinLat, int32& MaxLon, int32& MinLon, int32& LatitudeSegments, int32& LongitudeSegments) {
		LatitudeSegments = FMath::Max(FMath::RoundToInt(MaxLat * FMath::Pow(LODMul, LODIndex)), MinLat);
		LongitudeSegments = FMath::Max(FMath::RoundToInt(MaxLon * FMath::Pow(LODMul, LODIndex)), MinLon);
	}
	static bool GetSphereMesh(int32 SphereRadius, int32 LatitudeSegments, int32 LongitudeSegments, FRuntimeMeshRenderableMeshData& MeshData, TArray<UNoiseLayer*> InNoise);
	void UpdateMeshParameters(bool bAffectsCollision);

};
