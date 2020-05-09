// Fill out your copyright notice in the Description page of Project Settings.


// Include this to use the plane provider to test noise easier
//#include "Providers/RuntimeMeshProviderStatic.h"

#include "RuntimeSphereGenerator.h"
#include "Materials/Material.h"

ARuntimeSphereGenerator::ARuntimeSphereGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
	Radius = 200.f;
	Resolution = 10;

	//Noise = CreateDefaultSubobject<UNoiseLayer>(TEXT("Noise Layers"));	
}

void ARuntimeSphereGenerator::BeginPlay()
{
	Super::BeginPlay();
}


void ARuntimeSphereGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARuntimeSphereGenerator::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	//Uncomment the code below to use the plane to test noise easier. Handless less complexity

	//URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this, TEXT("RuntimeMeshProvider-Static"));
	//if (StaticProvider)
	//{
	//	GetRuntimeMeshComponent()->Initialize(StaticProvider);

	//	// This creates 3 positions for a triangle
	//	TArray<FVector> Positions;
	//	// This indexes our simple triangle
	//	TArray<int32> Triangles;
	//	// This creates 3 vertex colors
	//	TArray<FColor> Colors;
	//	TArray<FVector> EmptyNormals;
	//	TArray<FVector2D> EmptyTexCoords;
	//	TArray<FRuntimeMeshTangent> EmptyTangents;

	//	Noise->UpdateValues();
	//	for (int i = 0; i < Resolution; i++)
	//	{
	//		for (int j = 0; j < Resolution; j++)
	//		{
	//			FVector Vertex = FVector(i * 10, j * 10, 1.0f);
	//			Vertex.Z *= (Noise->GetHeightAt3DPoint(Vertex));
	//			Positions.Add(Vertex);
	//			EmptyTexCoords.Add(FVector2D(Vertex.X/10, Vertex.Y/10));
	//			if (i != Resolution - 1 && j != Resolution - 1)
	//			{
	//				Triangles.Add(Positions.Num() - 1);
	//				Triangles.Add(Positions.Num());
	//				Triangles.Add(Positions.Num() - 1 + Resolution);
	//				Triangles.Add(Positions.Num());
	//				Triangles.Add(Positions.Num() + Resolution);
	//				Triangles.Add(Positions.Num() - 1 + Resolution);
	//			}
	//		}
	//	}

	//	StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, EmptyNormals, EmptyTexCoords, Colors, EmptyTangents, ERuntimeMeshUpdateFrequency::Infrequent, true);
	//}


	if (PlanetProvider)
	{
		UpdateSphere();
	} 
	else
	{
		PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));
		PlanetProvider->SetSphereRadius(Radius);
		PlanetProvider->SetMaxLatitudeSegments(Resolution);
		PlanetProvider->SetMinLatitudeSegments(Resolution);
		PlanetProvider->SetMaxLongitudeSegments(Resolution);
		PlanetProvider->SetMinLongitudeSegments(Resolution);

		Noise.Add(NewObject<UNoiseLayer>(this, TEXT("NoiseLayer")));
		Noise.Add(NewObject<UNoiseLayer>(this, TEXT("NoiseLayer")));

		if (Noise.Num() != 0)
		{
			for (UNoiseLayer* NoiseLayer : Noise)
			{
				NoiseLayer->UpdateValues();
			}
			PlanetProvider->SetNoise(Noise);
		}
		GetRuntimeMeshComponent()->Initialize(PlanetProvider);
	}
	   	 
}

void ARuntimeSphereGenerator::GenerateSphere()
{
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));
	PlanetProvider->Initialize();

	//PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("ProceduralPlanetSettings"));
	//if (StaticProvider && SphereData)
	//{
	//	FRuntimeMeshSectionProperties MeshProperties;
	//	MeshProperties.bCastsShadow = true;
	//	MeshProperties.bIsVisible = true;
	//	MeshProperties.MaterialSlot = 0;
	//	MeshProperties.bWants32BitIndices = true;
	//	MeshProperties.bUseHighPrecisionTangents = true;
	//	MeshProperties.bUseHighPrecisionTexCoords = true;
	//	MeshProperties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

	//	StaticProvider->CreateSection(0,0, MeshProperties,*SphereData->MeshData,true);
	//}
}

void ARuntimeSphereGenerator::UpdateSphere()
{
	// Update it
	PlanetProvider->SetSphereRadius(Radius);
	PlanetProvider->SetMaxLatitudeSegments(Resolution);
	PlanetProvider->SetMinLatitudeSegments(Resolution);
	PlanetProvider->SetMaxLongitudeSegments(Resolution);
	PlanetProvider->SetMinLongitudeSegments(Resolution);
	if (Noise.Num() != 0)
	{
		for (UNoiseLayer* NoiseLayer : Noise)
		{
			NoiseLayer->UpdateValues();
		}
		PlanetProvider->SetNoise(Noise);
	}
	PlanetProvider->MarkSectionDirty(0, 0);
}

#if WITH_EDITOR
void ARuntimeSphereGenerator::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		if (PropertyChangedEvent.Property->GetFName() == FName("Radius") ||
			PropertyChangedEvent.Property->GetFName() == FName("Resolution") ||
			PropertyChangedEvent.Property->GetFName() == FName("Noise"))
		{
			//UpdateSphere();
		}
	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif