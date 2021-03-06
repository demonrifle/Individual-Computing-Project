// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetActor.h"
#include "Materials/Material.h"
#include "..\Public\ProceduralPlanetActor.h"
#include "Editor/EditorEngine.h"

DECLARE_CYCLE_STAT(TEXT("Actor - Updating"), STAT_UpdatePlanet, STATGROUP_PlanetActor);

AProceduralPlanetActor::AProceduralPlanetActor()
{
	
}

void AProceduralPlanetActor::BeginPlay()
{
	Super::BeginPlay();

	if (PlanetSettings && PlanetProvider)

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

	if (PlanetSettings && PlanetProvider)
	{
		if (PlanetSettings->EnableRealtimeUpdate)
		{
			UpdateSphere();
		}
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
	PlanetProvider->SetLODMultiplier(0.5f);

	// Initialize provider with the RMC and start the chain call to generate the mesh
	GetRuntimeMeshComponent()->Initialize(PlanetProvider);
}

// This method should only be called when both provider and settings are initialized
void AProceduralPlanetActor::UpdateSphere()
{
	SCOPE_CYCLE_COUNTER(STAT_UpdatePlanet);

	// Register for undo
	FString TransactionString = TEXT("Updating SPhere");
	const TCHAR* Transaction = *TransactionString;
	GEngine->BeginTransaction(Transaction, FText::FromString(TransactionString), this);
	
	// Update noise if it exists
	PlanetSettings->UpdateNoiseSettings();
	// Update provider for reconstruction
	PlanetProvider->UpdatePlanet();
	   	 
	// End undoable actions
	GEngine->EndTransaction();
}

FReply AProceduralPlanetActor::Randomize()
{	

	// Register for undo
	FString TransactionString = TEXT("Updating SPhere");
	const TCHAR* Transaction = *TransactionString;
	GEngine->BeginTransaction(Transaction, FText::FromString(TransactionString), this);
	
	PlanetSettings->Randomize();
	UpdateSphere();

	// End undoable actions
	GEngine->EndTransaction();

	return FReply::Handled();
}


#if WITH_EDITOR
void AProceduralPlanetActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif