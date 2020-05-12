// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetMeshProvider.h"
#include "DVector.h"
#include "RuntimeMeshComponentPlugin.h"

UProceduralPlanetMeshProvider::UProceduralPlanetMeshProvider()
	: MaxLOD(0)
		, SphereRadius(100.0f)
		, MaxLatitudeSegments(32)
		, MinLatitudeSegments(8)
		, MaxLongitudeSegments(16)
		, MinLongitudeSegments(5)
		, LODMultiplier(0.75)
		, SphereMaterial(nullptr)
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

int32 UProceduralPlanetMeshProvider::GetMaxLatitudeSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MaxLatitudeSegments;
}

void UProceduralPlanetMeshProvider::SetMaxLatitudeSegments(int32 InMaxLatitudeSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MaxLatitudeSegments = InMaxLatitudeSegments;
	UpdateMeshParameters(false);
}

int32 UProceduralPlanetMeshProvider::GetMinLatitudeSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MinLatitudeSegments;
}

void UProceduralPlanetMeshProvider::SetMinLatitudeSegments(int32 InMinLatitudeSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MinLatitudeSegments = InMinLatitudeSegments;
	UpdateMeshParameters(false);
}

int32 UProceduralPlanetMeshProvider::GetMaxLongitudeSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MaxLongitudeSegments;
}

void UProceduralPlanetMeshProvider::SetMaxLongitudeSegments(int32 InMaxLongitudeSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MaxLongitudeSegments = InMaxLongitudeSegments;
	UpdateMeshParameters(false);
}

int32 UProceduralPlanetMeshProvider::GetMinLongitudeSegments() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MinLongitudeSegments;
}

void UProceduralPlanetMeshProvider::SetMinLongitudeSegments(int32 InMinLongitudeSegments)
{
	FScopeLock Lock(&PropertySyncRoot);
	MinLongitudeSegments = InMinLongitudeSegments;
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

TArray<UNoiseLayer*> UProceduralPlanetMeshProvider::GetNoise() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return Noise;
}

void UProceduralPlanetMeshProvider::SetNoise(TArray<UNoiseLayer*> InNoiseLayer)
{
	FScopeLock Lock(&PropertySyncRoot);

	Noise = InNoiseLayer;
	UpdateMeshParameters(true);
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
	int32 TempMinLat, TempMaxLat;
	int32 TempMinLong, TempMaxLong;
	float TempLODMultiplier;

	// Set temp variables
	GetShapeParams(TempRadius, TempMinLat, TempMaxLat, TempMinLong, TempMaxLong, TempLODMultiplier);

	// Set segements for LOD
	int32 LatSegments, LonSegments;
	GetSegmentsForLOD(LODIndex, TempLODMultiplier, TempMaxLat, TempMinLat, TempMaxLong, TempMinLong, LatSegments, LonSegments);

	// Build up Section Mesh 
	return GetSphereMesh(TempRadius, LatSegments, LonSegments, MeshData, ProceduralPlanetSettings);
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
void UProceduralPlanetMeshProvider::GetShapeParams(float& OutRadius, int32& OutMinLatitudeSegments, int32& OutMaxLatitudeSegments, int32& OutMinLongitudeSegments, int32& OutMaxLongitudeSegments, float& OutLODMultiplier)
{
	FScopeLock Lock(&PropertySyncRoot);
	OutRadius = SphereRadius;
	OutMinLatitudeSegments = MinLatitudeSegments;
	OutMaxLatitudeSegments = MaxLatitudeSegments;
	OutMinLongitudeSegments = MinLongitudeSegments;
	OutMaxLongitudeSegments = MaxLongitudeSegments;
	OutLODMultiplier = LODMultiplier;
}


/*
 Get the maximum number of available LODs given the maximum and minimum segments.

*/
int32 UProceduralPlanetMeshProvider::GetMaxNumberOfLODs()
{
	FScopeLock Lock(&PropertySyncRoot);
	int32 MaxLODs = 1;
	float CurrentLatitudeSegments = MaxLatitudeSegments;
	float CurrentLongitudeSegments = MaxLongitudeSegments;

	// Up to MaxLODs - 8
	while (MaxLODs < RUNTIMEMESH_MAXLODS)
	{
		// Multiply current segments by the LODMultiplier to get segments at next stage
		CurrentLatitudeSegments *= LODMultiplier;
		CurrentLongitudeSegments *= LODMultiplier;

		// Have we gone far enough?
		// If lower than the minimum segments possible - stop
		if (CurrentLatitudeSegments <= MinLatitudeSegments && CurrentLongitudeSegments <= MinLongitudeSegments)
		{
			MaxLODs++;
			break;
		}

		//Else increase MaxLODs and start again
		MaxLODs++;
	}

	return MaxLODs;
}

//Calculate screen size that object should be before it's rendered
float UProceduralPlanetMeshProvider::CalculateScreenSize(int32 LODIndex)
{
	FScopeLock Lock(&PropertySyncRoot);
	float ScreenSize = FMath::Pow(LODMultiplier, LODIndex);

	return ScreenSize;
}

// Calculate actual Mesh data.
bool UProceduralPlanetMeshProvider::GetSphereMesh(int32 SphereRadius, int32 LatitudeSegments, int32 LongitudeSegments, FRuntimeMeshRenderableMeshData& MeshData, UProceduralPlanetSettings* PlanetSettings)
{
	TArray<FVector> LatitudeVerts;
	TArray<FVector> TangentVerts;
	int32 TrisOrder[6] = { 0, 1, LatitudeSegments + 1, 1, LatitudeSegments + 2, LatitudeSegments + 1 };
	//Baked trigonometric data to avoid computing it too much (sin and cos are expensive !)
	//Set array size
	LatitudeVerts.SetNumUninitialized(LatitudeSegments + 1);
	TangentVerts.SetNumUninitialized(LatitudeSegments + 1);
	//For each latitude segment + 1 due to poles
	for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments + 1; LatitudeIndex++)
	{
		float angle = LatitudeIndex * 2.f * PI / LatitudeSegments;
		float x, y;
		FMath::SinCos(&y, &x, angle);
		LatitudeVerts[LatitudeIndex] = FVector(x, y, 0);
		FMath::SinCos(&y, &x, angle + PI / 2.f);
		TangentVerts[LatitudeIndex] = FVector(x, y, 0);
	}
	//Making the verts
	for (int32 LongitudeIndex = 0; LongitudeIndex < LongitudeSegments + 1; LongitudeIndex++) //This is one more vert than geometrically needed but this avoid having to make wrap-around code
	{
		float angle = LongitudeIndex * PI / LongitudeSegments;
		float z, r;
		FMath::SinCos(&r, &z, angle);
		for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments + 1; LatitudeIndex++) //In total, we only waste (2*LatitudeSegments + LongitudeSegments - 2) vertices but save LatitudeSegments*LongitudeSegments operations
		{
			FVector Normal = LatitudeVerts[LatitudeIndex] * r + FVector(0, 0, z);
			FVector Position = Normal;

			// Valide Planet Settings
			if (PlanetSettings)
			{
				if (PlanetSettings->NoiseSettings.Num() != 0)
				{
					double NoiseValue = 0.f;
					DVector Vector = DVector(Position);
					for (UNoiseLayer* NoiseLayer : PlanetSettings->NoiseSettings)
					{
						// Validate Noise Layer
						if (NoiseLayer)
						{
							NoiseValue += NoiseLayer->GetHeightAt3DPoint(Vector);
						}
					}
					Position *= (SphereRadius + NoiseValue);
				}
			}
			else
			{
				Position *= SphereRadius;
			}

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, TangentVerts[LatitudeIndex]);
			MeshData.TexCoords.Add(FVector2D((float)LatitudeIndex / LatitudeSegments, (float)LongitudeIndex / LongitudeSegments));
			MeshData.Colors.Add(FColor::White);
		}
	}
	//Creating the tris
	for (int32 LongitudeIndex = 0; LongitudeIndex < LongitudeSegments; LongitudeIndex++)
	{
		for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments; LatitudeIndex++)
		{
			int32 TrisNumber = LatitudeIndex + LongitudeIndex * (LatitudeSegments + 1);
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
