// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

// This class serves as the AActor object for use in editor and during runtime for the planet generation
// It has a PlanetSettings object and a Provider, which builds the desired mesh
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshActor.h"
#include "ProceduralPlanetMeshProvider.h"
#include "ProceduralPlanetSettings.h"
#include "NoiseLayer.h"

#include "ProceduralPlanetActor.generated.h"


UCLASS()
class PROCEDURALPLANETGENERATOR_API AProceduralPlanetActor : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:


	UPROPERTY(VisibleAnywhere, Instanced, Category = "Planet Settings")
		UProceduralPlanetSettings* PlanetSettings;

	UProceduralPlanetMeshProvider* PlanetProvider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Default Constructor
	AProceduralPlanetActor();

	// Initialize function for all pointer initializations and default values.
	// This should be always called first after a new object is created
	void Initialize(bool IsRandom);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// OnConstruction reconstructs the whole mesh during every change and whenever an object is spawned or moved.
	virtual void OnConstruction(const FTransform& Transform) override;


	// The update of the sphere takes place after all initialization is done.
	// It updates all values according to any outside changes and updates all objects interally
	void UpdateSphere();

	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};