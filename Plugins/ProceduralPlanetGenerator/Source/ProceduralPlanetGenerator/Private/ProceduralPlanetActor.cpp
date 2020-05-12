// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetActor.h"
#include "Materials/Material.h"
#include "..\Public\ProceduralPlanetActor.h"

AProceduralPlanetActor::AProceduralPlanetActor()
{
}

void AProceduralPlanetActor::BeginPlay()
{
	Super::BeginPlay();
}


void AProceduralPlanetActor::Initialize(bool IsRandom)
{
	// Sets whether or not Tick event will affect this actor
	PrimaryActorTick.bCanEverTick = true;

	// Initialize settings object
	PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("PlanetSettings"));

	// Initialize RMC custom provider
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));


	if (IsRandom)
	{
		PlanetSettings->Initialize(IsRandom);
		PlanetSettings->UpdateNoiseSettings();

		PlanetProvider->SetProceduralPlanetSettings(PlanetSettings);
		PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
		PlanetProvider->SetMaxLatitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMinLatitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMaxLongitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMinLongitudeSegments(PlanetSettings->Resolution);

	}
	else
	{
		PlanetSettings->Initialize(IsRandom);
		PlanetSettings->UpdateNoiseSettings();

		PlanetProvider->SetProceduralPlanetSettings(PlanetSettings);
		PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
		PlanetProvider->SetMaxLatitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMinLatitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMaxLongitudeSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMinLongitudeSegments(PlanetSettings->Resolution);
	}

	// Initialize provider and starts the chain calls internally in the RMC to generate the mesh
	GetRuntimeMeshComponent()->Initialize(PlanetProvider);

}

void AProceduralPlanetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralPlanetActor::OnConstruction(const FTransform & Transform)
{
	// Call parent function
	Super::OnConstruction(Transform);

	// If 
	if (!PlanetProvider || !PlanetSettings)
	{
		GenerateSphere();
	}
	else
	{
		UpdateSphere();
	}

}

void AProceduralPlanetActor::GenerateSphere()
{
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));
	PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("PlanetSettings"));

	PlanetProvider->SetProceduralPlanetSettings(PlanetSettings);
	PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
	PlanetProvider->SetMaxLatitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinLatitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMaxLongitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinLongitudeSegments(PlanetSettings->Resolution);

	if (PlanetSettings->NoiseSettings.Num() != 0)
	{
		PlanetSettings->UpdateNoiseSettings();
	}

	GetRuntimeMeshComponent()->Initialize(PlanetProvider);
}

void AProceduralPlanetActor::UpdateSphere()
{
	// Update it
	PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
	PlanetProvider->SetMaxLatitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinLatitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMaxLongitudeSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinLongitudeSegments(PlanetSettings->Resolution);

	// Update noise if it exists
	if (PlanetSettings->NoiseSettings.Num() != 0)
	{
		PlanetSettings->UpdateNoiseSettings();
	}

	// Mark provider for reconstruction
	PlanetProvider->MarkSectionDirty(0, 0);
}

#if WITH_EDITOR
void AProceduralPlanetActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
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