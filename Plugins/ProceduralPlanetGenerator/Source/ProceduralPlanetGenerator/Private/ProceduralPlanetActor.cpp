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

	// Declare settings object
	PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("PlanetSettings"));

	// Declare RMC custom provider
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));

	// Initialize variables, if IsRandom values will be randomized
	PlanetSettings->Initialize(IsRandom);
	PlanetSettings->UpdateNoiseSettings();

	PlanetProvider->SetProceduralPlanetSettings(PlanetSettings);
	PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
	
	PlanetProvider->SetMaxSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinSegments(PlanetSettings->Resolution);
	

	// Initialize provider and start the chain call internally in the RMC to generate the mesh
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

	// Validate 
	// Check settings and provider are initialized
	if (PlanetProvider && PlanetSettings)
	{
		UpdateSphere();
	}
	// Else initialize blank planet
	else
	{
		Initialize(false);
	}

}

void AProceduralPlanetActor::GenerateSphere()
{
	// Validate all objects before creating new to ensure no memory loss
	if (!PlanetProvider)
	PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("PlanetSettings"));

	if (!PlanetSettings)
	{
		PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));

		PlanetProvider->SetProceduralPlanetSettings(PlanetSettings);
		PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
		PlanetProvider->SetMaxSegments(PlanetSettings->Resolution);
		PlanetProvider->SetMinSegments(PlanetSettings->Resolution);

		if (PlanetSettings->NoiseSettings.Num() != 0)
		{
			PlanetSettings->UpdateNoiseSettings();
		}

		GetRuntimeMeshComponent()->Initialize(PlanetProvider);
	}

}

// This method should only be called when both provider and settings are initialized
void AProceduralPlanetActor::UpdateSphere()
{
	
	// Update it
	PlanetProvider->SetSphereRadius(PlanetSettings->Radius);
	PlanetProvider->SetMaxSegments(PlanetSettings->Resolution);
	PlanetProvider->SetMinSegments(PlanetSettings->Resolution);

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