// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshActor.h"
#include "ProceduralPlanetMeshProvider.h"
#include "ProceduralPlanetSettings.h"
#include "NoiseLayer.h"

#include "RuntimeSphereGenerator.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALPLANETGENERATOR_API ARuntimeSphereGenerator : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARuntimeSphereGenerator();
	//ARuntimeSphereGenerator(float Radius, float Resolution);

	UPROPERTY(VisibleAnywhere, Instanced, Category = "Planet Settings")
		UProceduralPlanetSettings* PlanetSettings;

	UProceduralPlanetMeshProvider* PlanetProvider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void GenerateSphere();
	void UpdateSphere();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
