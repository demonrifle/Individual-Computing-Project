// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeSphereGenerator.h"
#include "Materials/Material.h"

ARuntimeSphereGenerator::ARuntimeSphereGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
	Radius = 200.f;
	Resolution = 10;

	Noise = CreateDefaultSubobject<UNoiseLayer>(TEXT("Noise Layers"));	
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

		if (Noise)
		{
			Noise->UpdateValues();
			PlanetProvider->SetNoise(Noise);
		}
		GetRuntimeMeshComponent()->Initialize(PlanetProvider);
	}

	// Plane provider for test purposes


}

void ARuntimeSphereGenerator::GenerateSphere()
{
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));
	PlanetProvider->Initialize();
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
	if (Noise)
	{
		Noise->UpdateValues();
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