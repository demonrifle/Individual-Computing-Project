// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetMeshProvider.h"
#include "DVector.h"
#include "RuntimeMeshComponentPlugin.h"

DECLARE_CYCLE_STAT(TEXT("Provider - Getting LOD Mesh Section"), STAT_PlanetProviderGetMeshSectionForLOD, STATGROUP_PlanetProvider);

UProceduralPlanetMeshProvider::UProceduralPlanetMeshProvider()
{
	// Default empty constructor
}

void UProceduralPlanetMeshProvider::Initialize(UProceduralPlanetSettings* InProceduralPlanetSettings)
{
	ProceduralPlanetSettings = InProceduralPlanetSettings;
	// IsValidSettings settings
	IsValidSettings();

	MinSegments = 32;
	LODMultiplier = 0.50;
	MaxLOD = GetMaxNumberOfLODs() - 1;

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
		LODMultiplier = 0.50f;
		return;
	}
	LODMultiplier = InLODMultiplier;
	UpdateMeshParameters(false);
}

//UProceduralPlanetSettings UProceduralPlanetMeshProvider::GetProceduralPlanetSettings() const
//{
//	FScopeLock Lock(&PropertySyncRoot);
//	UProceduralPlanetSettings* Temp = ProceduralPlanetSettings;
//	return Temp;
//}


void UProceduralPlanetMeshProvider::Initialize_Implementation()
{
	if (!IsValidSettings()) return;

	// Setup material
	SetupMaterialSlot(0, FName("Sphere Base"), ProceduralPlanetSettings->GetSphereMaterial());

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
		// Use 32 BitIndices always to avoid overflow
		Properties.bWants32BitIndices = true;
		Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

		// Setup section
		CreateSection(LODIndex, 0, Properties);
	}
}

void UProceduralPlanetMeshProvider::UpdatePlanet()
{
	LODMultiplier = 0.5;
	//MaxLOD = GetMaxNumberOfLODs() - 1; 
	MarkAllLODsDirty();
	MarkCollisionDirty();
}

bool UProceduralPlanetMeshProvider::GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{
	SCOPE_CYCLE_COUNTER(STAT_PlanetProviderGetMeshSectionForLOD);

	// We should only ever be queried for section 0
	check(SectionId == 0 && LODIndex <= MaxLOD);

	if (!IsValidSettings()) return false;
	Timer.Tick();
	// Setup material
	SetupMaterialSlot(0, FName("Sphere Base"), ProceduralPlanetSettings->GetSphereMaterial());

	// Set segments for LOD
	int32 LODSegments = GetSegmentsForLOD(LODIndex);

	bool result = GetSphereMesh(LODSegments, MeshData);// , TempSettings);

	UE_LOG(ProceduralPlanetModule, Verbose, TEXT("Generating Section Mesh for LOD %d with %d Segments took : %.6fms to update"), LODIndex, LODSegments, Timer.Tock());

	
	ProceduralPlanetSettings->PrintLayerAverageSpeed();

	// Build up Section Mesh for LOD
	return result;
}

FRuntimeMeshCollisionSettings UProceduralPlanetMeshProvider::GetCollisionSettings_Implementation()
{
	FRuntimeMeshCollisionSettings Settings;
	Settings.bUseAsyncCooking = false;
	Settings.bUseComplexAsSimple = false;
	if (!IsValidSettings()) Settings.Spheres.Emplace(300.0f);
	else Settings.Spheres.Emplace(ProceduralPlanetSettings->Radius);

	return Settings;
}

// Get render bounds for planet
FBoxSphereBounds UProceduralPlanetMeshProvider::GetBounds_Implementation()
{
	if (!IsValidSettings()) return FBoxSphereBounds(FSphere(FVector::ZeroVector, 300.0f));
	// Return bounds will encapsulate the planet with the heighest available point from noise layers
	float BoundsRadius = ProceduralPlanetSettings->Radius + ProceduralPlanetSettings->GetHeightAt3DPointMax();
	return FBoxSphereBounds(FSphere(FVector::ZeroVector, BoundsRadius));
	
}

bool UProceduralPlanetMeshProvider::IsThreadSafe_Implementation()
{
	return true;
}

int32 UProceduralPlanetMeshProvider::GetMaxNumberOfLODs()
{
	if (!IsValidSettings()) return 1;
	FScopeLock Lock(&PropertySyncRoot);

	int32 MaxLODs = 1;
	float CurrentSegments = FMath::Abs(ProceduralPlanetSettings->Resolution);

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
	float ScreenSize = FMath::Pow(LODMultiplier, LODIndex) / 2;

	return ScreenSize;
}

int32 UProceduralPlanetMeshProvider::GetSegmentsForLOD(int32 LODIndex)
{
	IsValidSettings();
	// Get segments for required LOD
	int32 Segments = FMath::Abs(ProceduralPlanetSettings->Resolution);
	if (LODIndex < MaxLOD)
	{
		return 	Segments *= FMath::Pow(LODMultiplier, LODIndex);
	}
	else return MinSegments;
	
}

// Calculate actual Mesh data.
bool UProceduralPlanetMeshProvider::GetSphereMesh(int32 Segments, FRuntimeMeshRenderableMeshData& MeshData)//, UProceduralPlanetSettings PlanetSettings)
{
	TArray<FVector> LatitudeVerts;
	TArray<FVector> TangentVerts;
	//Baked trigonometric data to avoid computing it too much (sin and cos are expensive !)
	int32 TrisOrder[6] = { 0, 1, Segments + 1, 1, Segments + 2, Segments + 1 };
	int32 SphereRadius = ProceduralPlanetSettings->Radius;
	float MaxHeight = ProceduralPlanetSettings->GetHeightAt3DPointMax();

	//Set array size
	LatitudeVerts.SetNumUninitialized(Segments + 1);
	TangentVerts.SetNumUninitialized(Segments + 1);
	//For each latitude segment + 1 due to poles
	for (int32 LatitudeIndex = 0; LatitudeIndex < Segments + 1; LatitudeIndex++)
	{
		// Calculate latitude circles and radius
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
			FColor VertexColor;

			// Get height for point
			DVector Vector = DVector(Position);
			double NoiseValue = ProceduralPlanetSettings->GetHeightAt3DPointForAllLayers(Vector);
			Position *= (SphereRadius + NoiseValue);

			// Get vertex color for point
			VertexColor = ProceduralPlanetSettings->GetVertexColorFor3DHeight(NoiseValue, MaxHeight);

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, TangentVerts[LatitudeIndex]);
			MeshData.TexCoords.Add(FVector2D((float)LatitudeIndex / Segments, (float)LongitudeIndex / Segments));
			MeshData.Colors.Add(VertexColor);
			
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

void UProceduralPlanetMeshProvider::UpdateMeshParameters(bool bAffectsCollision)
{

	MaxLOD = GetMaxNumberOfLODs() - 1;

	MarkAllLODsDirty();
	if (bAffectsCollision)
	{
		MarkCollisionDirty();
	}
}

bool UProceduralPlanetMeshProvider::IsValidSettings()
{
	// If not initialized
	if (!ProceduralPlanetSettings)
	{
		// Log out 
		UE_LOG(LogTemp, Verbose, TEXT("RMC ProceduralPlanetProvider Initialization for Object: ProceduralPlanetSettings object is uninitialized. Object is being destroyed."));
		return false;
	}
	return true;
}