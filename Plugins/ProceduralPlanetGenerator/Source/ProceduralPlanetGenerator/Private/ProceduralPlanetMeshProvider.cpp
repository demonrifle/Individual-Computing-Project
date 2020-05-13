// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetMeshProvider.h"
#include "DVector.h"
#include "RuntimeMeshComponentPlugin.h"

UProceduralPlanetMeshProvider::UProceduralPlanetMeshProvider()
	: MaxLOD(0)
		, SphereRadius(100.0f)
		, MaxSegments(32)
		, MinSegments(8)
		, LODMultiplier(0.50)
		, SphereMaterial(nullptr)
		, ProceduralPlanetSettings(nullptr)
{
	MaxLOD = GetMaxNumberOfLODs() - 1;
}

float UProceduralPlanetMeshProvider::GetSphereRadius() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return SphereRadius;
}

void UProceduralPlanetMeshProvider::SetSphereRadius(float InSphereRadius)
{
	FScopeLock Lock(&PropertySyncRoot);
	SphereRadius = InSphereRadius;
	UpdateMeshParameters(true);
}

int32 UProceduralPlanetMeshProvider::GetMaxSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MaxSegments;
}

void UProceduralPlanetMeshProvider::SetMaxSegments(int32 InMaxSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MaxSegments = InMaxSegments;
	UpdateMeshParameters(false);
}

int32 UProceduralPlanetMeshProvider::GetMinSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MinSegments;
}

void UProceduralPlanetMeshProvider::SetMinSegments(int32 InMinSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MinSegments = InMinSegments;
	UpdateMeshParameters(false);
}

float UProceduralPlanetMeshProvider::GetLODMultiplier() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return LODMultiplier;
}

void UProceduralPlanetMeshProvider::SetLODMultiplier(float InLODMultiplier)
{
	FScopeLock Lock(&PropertySyncRoot);

	//If the LODMultiplier is greater than one, there'd be infinite LODs as it would diverge. (It's a geometric series)
	if (InLODMultiplier >= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("RMC Sphere Provider(%d): LODMultiplier was set greater than or equal to 1 ! Aborting ! Value : %f"), FPlatformTLS::GetCurrentThreadId(), InLODMultiplier);
		return;
	}
	LODMultiplier = InLODMultiplier;
	UpdateMeshParameters(false);
}

UMaterialInterface* UProceduralPlanetMeshProvider::GetSphereMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return SphereMaterial;
}

void UProceduralPlanetMeshProvider::SetSphereMaterial(UMaterialInterface* InSphereMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	SphereMaterial = InSphereMaterial;
	//Re-setup the material again
	this->SetupMaterialSlot(0, FName("Sphere Base"), SphereMaterial);
}

UProceduralPlanetSettings * UProceduralPlanetMeshProvider::GetProceduralPlanetSettings() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return ProceduralPlanetSettings;
}

void UProceduralPlanetMeshProvider::SetProceduralPlanetSettings(UProceduralPlanetSettings * InProceduralPlanetSettings)
{
	FScopeLock Lock(&PropertySyncRoot);

	ProceduralPlanetSettings = InProceduralPlanetSettings;
	UpdateMeshParameters(true);

}

void UProceduralPlanetMeshProvider::Initialize_Implementation()
{
	SetupMaterialSlot(0, FName("Sphere Base"), SphereMaterial);

	// Setup LODs
	TArray<FRuntimeMeshLODProperties> LODs;
	for (int32 LODIndex = 0; LODIndex <= MaxLOD; LODIndex++)
	{
		FRuntimeMeshLODProperties LODProperties;
		LODProperties.ScreenSize = CalculateScreenSize(LODIndex);
		LODs.Add(LODProperties);
	}
	ConfigureLODs(LODs);

	// Setup sections
	for (int32 LODIndex = 0; LODIndex <= MaxLOD; LODIndex++)
	{
		// Declare properties
		FRuntimeMeshSectionProperties Properties;
		Properties.bCastsShadow = true;
		Properties.bIsVisible = true;
		Properties.MaterialSlot = 0;
		Properties.bWants32BitIndices = true;
		Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

		// Setup section
		CreateSection(LODIndex, 0, Properties);
	}
}

bool UProceduralPlanetMeshProvider::GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{
	UE_LOG(LogTemp, Verbose, TEXT("RMC Sphere Provider(%d): Getting LOD:%d Section:%d"), FPlatformTLS::GetCurrentThreadId(), LODIndex, SectionId);

	// We should only ever be queried for section 0
	check(SectionId == 0 && LODIndex <= MaxLOD);

	// Temporary variables declaration
	float TempRadius;
	int32 TempMin, TempMax;
	float TempLODMultiplier;

	// Set temp variables
	GetShapeParams(TempRadius, TempMin, TempMax, TempLODMultiplier);

	// Set segements for LOD
	int32 Segments;
	GetSegmentsForLOD(LODIndex, TempLODMultiplier, TempMax, TempMin, Segments);

	// Build up Section Mesh 
	return GetSphereMesh(TempRadius, Segments,  MeshData, ProceduralPlanetSettings);
}

FRuntimeMeshCollisionSettings UProceduralPlanetMeshProvider::GetCollisionSettings_Implementation()
{
	FRuntimeMeshCollisionSettings Settings;
	Settings.bUseAsyncCooking = false;
	Settings.bUseComplexAsSimple = false;

	Settings.Spheres.Emplace(GetSphereRadius());

	return Settings;
}

// Get render bounds for planet
FBoxSphereBounds UProceduralPlanetMeshProvider::GetBounds_Implementation()
{
	return FBoxSphereBounds(FSphere(FVector::ZeroVector, SphereRadius));
}

bool UProceduralPlanetMeshProvider::IsThreadSafe_Implementation()
{
	return true;
}

//Threadsafe getter
void UProceduralPlanetMeshProvider::GetShapeParams(float& OutRadius, int32& OutMinSegments, int32& OutMaxSegments, float& OutLODMultiplier)
{
	FScopeLock Lock(&PropertySyncRoot);
	OutRadius = SphereRadius;
	OutMinSegments = MinSegments;
	OutMaxSegments = MaxSegments;
	OutLODMultiplier = LODMultiplier;
}


/*
 Get the maximum number of available LODs given the maximum and minimum segments.

*/
int32 UProceduralPlanetMeshProvider::GetMaxNumberOfLODs()
{
	FScopeLock Lock(&PropertySyncRoot);
	int32 MaxLODs = 1;
	float CurrentSegments = MaxSegments;

	// Up to MaxLODs - 8
	while (MaxLODs < RUNTIMEMESH_MAXLODS)
	{
		// Multiply current segments by the LODMultiplier to get segments at next stage
		CurrentSegments *= LODMultiplier;

		// Have we gone far enough?
		// If lower than the minimum segments possible - stop
		if (CurrentSegments <= MinSegments)
		{
			MaxLODs++;
			break;
		}

		//Else increase MaxLODs and start again
		MaxLODs++;
	}

	return MaxLODs;
}

//Calculate screen size(%) that object should be before it's rendered
float UProceduralPlanetMeshProvider::CalculateScreenSize(int32 LODIndex)
{
	FScopeLock Lock(&PropertySyncRoot);
	float ScreenSize = FMath::Pow(LODMultiplier, LODIndex);

	return ScreenSize;
}

// Calculate actual Mesh data.
bool UProceduralPlanetMeshProvider::GetSphereMesh(int32 SphereRadius, int32 Segments, FRuntimeMeshRenderableMeshData& MeshData, UProceduralPlanetSettings* PlanetSettings)
{
	TArray<FVector> LatitudeVerts;
	TArray<FVector> TangentVerts;
	int32 TrisOrder[6] = { 0, 1, Segments + 1, 1, Segments + 2, Segments + 1 };
	//Baked trigonometric data to avoid computing it too much (sin and cos are expensive !)
	//Set array size
	LatitudeVerts.SetNumUninitialized(Segments + 1);
	TangentVerts.SetNumUninitialized(Segments + 1);
	//For each latitude segment + 1 due to poles
	for (int32 LatitudeIndex = 0; LatitudeIndex < Segments + 1; LatitudeIndex++)
	{
		float angle = LatitudeIndex * 2.f * PI / Segments;
		float x, y;
		FMath::SinCos(&y, &x, angle);
		LatitudeVerts[LatitudeIndex] = FVector(x, y, 0);
		FMath::SinCos(&y, &x, angle + PI / 2.f);
		TangentVerts[LatitudeIndex] = FVector(x, y, 0);
	}
	//Making the verts
	for (int32 LongitudeIndex = 0; LongitudeIndex < Segments + 1; LongitudeIndex++) //This is one more vert than geometrically needed but this avoid having to make wrap-around code
	{
		float angle = LongitudeIndex * PI / Segments;
		float z, r;
		FMath::SinCos(&r, &z, angle);
		for (int32 LatitudeIndex = 0; LatitudeIndex < Segments + 1; LatitudeIndex++) //In total, we only waste (2*Segments + Segments - 2) vertices but save Segments*Segments operations
		{
			FVector Normal = LatitudeVerts[LatitudeIndex] * r + FVector(0, 0, z);
			FVector Position = Normal;

			// Valide Planet Settings
			if (PlanetSettings)
			{
				if (PlanetSettings->NoiseSettings.Num() != 0)
				{
					DVector Vector = DVector(Position);
					double NoiseValue = PlanetSettings->GetHeightAt3DPointForAllLayers(Vector);

					Position *= (SphereRadius + NoiseValue);
				}
			}
			else
			{
				Position *= SphereRadius;
			}

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, TangentVerts[LatitudeIndex]);
			MeshData.TexCoords.Add(FVector2D((float)LatitudeIndex / Segments, (float)LongitudeIndex / Segments));
			MeshData.Colors.Add(FColor::White);
		}
	}
	//Creating the tris
	for (int32 LongitudeIndex = 0; LongitudeIndex < Segments; LongitudeIndex++)
	{
		for (int32 LatitudeIndex = 0; LatitudeIndex < Segments; LatitudeIndex++)
		{
			int32 TrisNumber = LatitudeIndex + LongitudeIndex * (Segments + 1);
			for (int32 TrisIndex = 0; TrisIndex < 6; TrisIndex++)
			{
				MeshData.Triangles.Add(TrisOrder[TrisIndex] + TrisNumber);
			}
		}
	}
	return true;
}

// Update function
void UProceduralPlanetMeshProvider::UpdateMeshParameters(bool bAffectsCollision)
{
	MaxLOD = GetMaxNumberOfLODs() - 1;

	MarkAllLODsDirty();
	if (bAffectsCollision)
	{
		MarkCollisionDirty();
	}
}
