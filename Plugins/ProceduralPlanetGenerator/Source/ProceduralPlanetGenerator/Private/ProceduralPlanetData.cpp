// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralPlanetData.h"

ProceduralPlanetData::ProceduralPlanetData()
{
	MeshData = MakeShared<FRuntimeMeshRenderableMeshData>();
	Radius = 200.0f;
	Resolution = 10;
	Initialize();
}

ProceduralPlanetData::ProceduralPlanetData(float SphereRadius, uint32 SphereResolution)
{
	FRuntimeMeshSectionProperties Properties;
	Properties.bUseHighPrecisionTangents = true;
	Properties.bUseHighPrecisionTexCoords = true;
	Properties.bWants32BitIndices = true;

	MeshData = MakeShared<FRuntimeMeshRenderableMeshData>(Properties);
	Update(SphereRadius, SphereResolution);
}

ProceduralPlanetData::~ProceduralPlanetData()
{
}

void ProceduralPlanetData::Update(float SphereRadius, uint32 SphereResolution)
{
	Radius = SphereRadius;
	Resolution = SphereResolution;
	Initialize();
}


void ProceduralPlanetData::Initialize()
{
	//Clear Previous Data and Set New Size for Arrays
	MeshData->Reset();

	//int32 NewSize = Resolution * Resolution * 6;
	//MeshData->
	//MeshData.Normals.Reset(NewSize);
	//MeshData.UV0.Reset(NewSize);
	//MeshData.Positions.Reset(NewSize);
	//MeshData.Tangents.Reset(NewSize);
	//MeshData.VertexColors.Reset(NewSize);
	//MeshData.Triangles.Reset((NewSize - 2*Resolution + 1)*6);

	GetSphereMesh();

	////Build the 6 planes
	//MakePlaneData(FRotator(0.f, 0.f, 0.f));
	//MakePlaneData(FRotator(90.f, 0.f, 0.f));
	//MakePlaneData(FRotator(180.f, 0.f, 0.f));
	//MakePlaneData(FRotator(-90.f, 0.f, 0.f));
	//MakePlaneData(FRotator(90.f, 0.f, 90.f));
	//MakePlaneData(FRotator(90.f, 0.f, -90.f));
}

//void ProceduralPlanetData::MakePlaneData(FRotator Rotation)
//{
//	//Baked triangle order to save some calculations
//	uint32 TrisOrder[6] = { 0, 1, Resolution, 1, Resolution + 1, Resolution };
//	for (uint32 XIndex = 0; XIndex < Resolution; XIndex++)
//	{
//		for (uint32 YIndex = 0; YIndex < Resolution; YIndex++)
//		{
//			FVector Position;
//			Position.X = (XIndex / ((float)Resolution - 1) - 0.5f) * 2.f;
//			Position.Y = (YIndex / ((float)Resolution - 1) - 0.5f) * 2.f;
//			Position.Z = 1.f;
//			//Unit Sphere position
//			Position.Normalize();
//			//Add plane rotation
//			Position = Rotation.RotateVector(Position);
//			//Normals
//			MeshData.Normals.Add(Position);
//			//UV0
//			MeshData.UV0.Add(FVector2D(Position.X,Position.Y));
//			//Position
//			Position *= Radius;
//			MeshData.Positions.Add(Position);
//
//			//Calculate triangles
//			if (XIndex != Resolution - 1 && YIndex != Resolution - 1)
//			{
//				uint32 TrisNumber = MeshData.Positions.Num()-1;
//				for (int32 TrisIndex = 0; TrisIndex < 6; TrisIndex++)
//				{
//					MeshData.Triangles.Add(TrisOrder[TrisIndex] + TrisNumber);
//				}
//			}
//		}
//	}
//}

void ProceduralPlanetData::GetSphereMesh()
{
	int32 LatitudeSegments = Resolution;
	int32 LongitudeSegments = Resolution;
	
	TArray<FVector> LatitudeVerts;
	TArray<FVector> TangentVerts;
	int32 TrisOrder[6] = { 0, 1, LatitudeSegments + 1, 1, LatitudeSegments + 2, LatitudeSegments + 1 };
	//Baked trigonometric data to avoid computing it too much (sin and cos are expensive !)
	//Set array size
	LatitudeVerts.SetNumUninitialized(LatitudeSegments + 1);
	TangentVerts.SetNumUninitialized(LatitudeSegments + 1);
	//For each latitude segment + 1 due to poles
	for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments + 1; LatitudeIndex++)
	{
		float angle = LatitudeIndex * 2.f * PI / LatitudeSegments;
		float x, y;
		FMath::SinCos(&y, &x, angle);
		LatitudeVerts[LatitudeIndex] = FVector(x, y, 0);
		FMath::SinCos(&y, &x, angle + PI / 2.f);
		TangentVerts[LatitudeIndex] = FVector(x, y, 0);
	}
	//Making the verts
	for (int32 LongitudeIndex = 0; LongitudeIndex < LongitudeSegments + 1; LongitudeIndex++) //This is one more vert than geometrically needed but this avoid having to make wrap-around code
	{
		float angle = LongitudeIndex * PI / LongitudeSegments;
		float z, r;
		FMath::SinCos(&r, &z, angle);
		for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments + 1; LatitudeIndex++) //In total, we only waste (2*LatitudeSegments + LongitudeSegments - 2) vertices but save LatitudeSegments*LongitudeSegments operations
		{
			FVector Normal = LatitudeVerts[LatitudeIndex] * r + FVector(0, 0, z);
			FVector Position = Normal * Radius;
			//THIS IS WHERE YOU ADD THE NOISE
			MeshData->Positions.Add(Position);
			MeshData->Tangents.Add(Normal, TangentVerts[LatitudeIndex]);
			MeshData->TexCoords.Add(FVector2D((float)LatitudeIndex / LatitudeSegments, (float)LongitudeIndex / LongitudeSegments));
			MeshData->Colors.Add(FColor::White);
		}
	}
	//Creating the tris
	for (int32 LongitudeIndex = 0; LongitudeIndex < LongitudeSegments; LongitudeIndex++)
	{
		for (int32 LatitudeIndex = 0; LatitudeIndex < LatitudeSegments; LatitudeIndex++)
		{
			int32 TrisNumber = LatitudeIndex + LongitudeIndex * (LatitudeSegments + 1);
			for (int32 TrisIndex = 0; TrisIndex < 6; TrisIndex++)
			{
				MeshData->Triangles.Add(TrisOrder[TrisIndex] + TrisNumber);
			}
		}
	}
}
