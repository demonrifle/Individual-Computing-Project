// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Providers/RuntimeMeshProviderStatic.h"
#include "RuntimeMeshRenderable.h"

//TODO : Clear this struct from everywhere
struct ProceduralPlanetMeshData
{
	TArray<FVector> Positions;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FRuntimeMeshTangent> Tangents;
};


class PROCEDURALPLANETGENERATOR_API ProceduralPlanetData
{
public:
	float Radius;
	uint32 Resolution;

	//ProceduralPlanetMeshData MeshData;
	TSharedPtr<FRuntimeMeshRenderableMeshData> MeshData;

public:
	ProceduralPlanetData();
	ProceduralPlanetData(float SphereRadius, uint32 SphereResolution);
	~ProceduralPlanetData();
	void Update(float SphereRadius, uint32 SphereResolution);

private:
	void Initialize();
	void MakePlaneData(FRotator Rotation);
	void GetSphereMesh();
};
