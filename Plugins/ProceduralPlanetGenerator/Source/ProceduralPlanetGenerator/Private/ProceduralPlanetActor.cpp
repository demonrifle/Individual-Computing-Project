// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetActor.h"
#include "Materials/Material.h"
#include "..\Public\ProceduralPlanetActor.h"

AProceduralPlanetActor::AProceduralPlanetActor()
{
	//Initialize(false);
}

void AProceduralPlanetActor::BeginPlay()
{
	Super::BeginPlay();
	// Validate 
	// Check settings and provider are initialized
	if (PlanetProvider && PlanetSettings)
	{
		UpdateSphere();
	}
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
}

void AProceduralPlanetActor::Initialize(bool IsRandom)
{
	// Sets whether or not Tick event will affect this actor
	PrimaryActorTick.bCanEverTick = true;

	// Declare settings object
	PlanetSettings = NewObject<UProceduralPlanetSettings>(this, TEXT("ProceduralPlanetSettings"));
	// Initialize variables, if IsRandom values will be randomized
	PlanetSettings->Initialize(IsRandom);
	PlanetSettings->UpdateNoiseSettings();

	// Declare RMC custom provider
	PlanetProvider = NewObject<UProceduralPlanetMeshProvider>(this, TEXT("RuntimeMeshprovider-Planet"));
	// Initialize with Settings object to work as intended
	PlanetProvider->Initialize(PlanetSettings);

	// Initialize provider with the RMC and start the chain call to generate the mesh
	GetRuntimeMeshComponent()->Initialize(PlanetProvider);

}

// This method should only be called when both provider and settings are initialized
void AProceduralPlanetActor::UpdateSphere()
{
	// Update noise if it exists
	PlanetSettings->UpdateNoiseSettings();

	// Mark provider for reconstruction
	PlanetProvider->MarkAllLODsDirty();
	PlanetProvider->MarkCollisionDirty();
}

FReply AProceduralPlanetActor::Randomize()
{
	PlanetSettings->Randomize();
	UpdateSphere();
	return FReply::Handled();
}


#if WITH_EDITOR
void AProceduralPlanetActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif