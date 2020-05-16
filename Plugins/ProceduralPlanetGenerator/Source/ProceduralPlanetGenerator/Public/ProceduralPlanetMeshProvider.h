// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "ProceduralPlanetSettings.h"
#include "NoiseLayer.h"

#include "ProceduralPlanetMeshProvider.generated.h"


// This class extends the RMC provider behaviour to build the mesh for a procedural planet
UCLASS()
class PROCEDURALPLANETGENERATOR_API UProceduralPlanetMeshProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	// ScopeLock used for thread-safe getters and setters
	mutable FCriticalSection PropertySyncRoot;

	// Settings object to use for generation
	// This should always be initialized before use
		UProceduralPlanetSettings* ProceduralPlanetSettings;

		// Internal variables for setting up an LOD system. Only multiplier is editable with a setter
		int32 MinSegments;
		int32 MaxLOD;
		float LODMultiplier;


public:

	//UObject constructor
	UProceduralPlanetMeshProvider();

	// Initialization Method. Should always be called before attemtping to use the object.
	// Never call with a null pointer as it will generate nothing or crash.
	void Initialize(UProceduralPlanetSettings* InProceduralPlanetSettings);

	float GetLODMultiplier() const;
	void SetLODMultiplier(float InLODMultiplier);

protected:
	// Initialize implementation of the RMC. Gets called automtaically during GetRMC->Initialize()
	virtual void Initialize_Implementation() override;
	// Gets a mesh section for a given LOD. Currently only used with a single section.
	virtual bool GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	// Gets collision settings
	virtual FRuntimeMeshCollisionSettings GetCollisionSettings_Implementation() override;
	// Gets bounds for screen size and LOD
	virtual FBoxSphereBounds GetBounds_Implementation() override;
	virtual bool IsThreadSafe_Implementation() override;


private:
	// Calculates the maximum number of LODs that should be generated.
	int32 GetMaxNumberOfLODs();
	// Calcualtes the screen the object should be be for given LOD before rendering.
	float CalculateScreenSize(int32 LODIndex);
	// Gets segments to use during rendering for a given LOD 
	int32 GetSegmentsForLOD(int32 LODIndex);

	// Calculates the mesh for the sphere and adds to MeshData.
	// @param Segments Segments are used even thought the resolution can already be found in the PlanetSettings object since it is different for LODs
	bool GetSphereMesh(int32 Segments, FRuntimeMeshRenderableMeshData& MeshData);
	// Updates mesh. Can also update collision. Collision shouldn't be updated during LOD changes as it will only ever take affect at LOD 0 anyway, so it's needless calculations.
	void UpdateMeshParameters(bool bAffectsCollision);	
	// Validates the ProceduralPlanetSettings object. Should be called from any functions accessing it
	bool IsValidSettings();

};
