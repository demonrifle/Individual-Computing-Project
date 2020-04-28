// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeSphereGenerator.h"
#include "Providers/RuntimeMeshProviderStatic.h"
#include "Providers/RuntimeMeshProviderSphere.h"
#include "Materials/Material.h"

ARuntimeSphereGenerator::ARuntimeSphereGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
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

	//Initialize Data
	SphereData = MakeUnique<ProceduralPlanetData>(Radius, Resolution);//
	//SphereData = new ProceduralPlanetData(Radius, Resolution);

	//Initialize Provider
	StaticProvider = NewObject<URuntimeMeshProviderStatic>(this, TEXT("RuntimeMeshprovider-Static"));
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->SetupMaterialSlot(0, FName("Base Mat"), UMaterial::GetDefaultMaterial(MD_Surface));

	//Generate Mesh
	GenerateSphere();
}

void ARuntimeSphereGenerator::GenerateSphere()
{
	if (StaticProvider && SphereData)
	{
		FRuntimeMeshSectionProperties MeshProperties;
		MeshProperties.bCastsShadow = true;
		MeshProperties.bIsVisible = true;
		MeshProperties.MaterialSlot = 0;
		MeshProperties.bWants32BitIndices = true;
		MeshProperties.bUseHighPrecisionTangents = true;
		MeshProperties.bUseHighPrecisionTexCoords = true;
		MeshProperties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

		StaticProvider->CreateSection(0,0, MeshProperties,*SphereData->MeshData,true);

		//StaticProvider->CreateSectionFromComponents(
		//	0, //LOD Index
		//	0, //Section index
		//	0, //Material Slot
		//	SphereData->MeshData.Positions,
		//	SphereData->MeshData.Triangles,
		//	SphereData->MeshData.Normals,
		//	SphereData->MeshData.UV0,
		//	SphereData->MeshData.VertexColors,
		//	SphereData->MeshData.Tangents,
		//	ERuntimeMeshUpdateFrequency::Infrequent,
		//	true);//Create Collision
	}


	//if (SphereProvider)
	//{
	//	SphereProvider->SetSphereRadius(Radius);
	//	SphereProvider->SetMaxLongitudeSegments(Resolution-1);
	//	SphereProvider->SetMinLongitudeSegments(Resolution-1);
	//	SphereProvider->SetMaxLatitudeSegments(Resolution);
	//	SphereProvider->SetMinLatitudeSegments(Resolution);
	//

	//	GetRuntimeMeshComponent()->Initialize(SphereProvider);
	//}


}

void ARuntimeSphereGenerator::UpdateSphere()
{

	//Set Radius and Resolution
	SphereData->Update(Radius, Resolution);
	
	StaticProvider->UpdateSection(0, 0, *SphereData->MeshData);

	//StaticProvider->UpdateSectionFromComponents(
	//	0, //LOD Index
	//	0, //Section Index
	//	SphereData->MeshData.Positions,
	//	SphereData->MeshData.Triangles,
	//	SphereData->MeshData.Normals,
	//	SphereData->MeshData.UV0,
	//	SphereData->MeshData.VertexColors,
	//	SphereData->MeshData.Tangents);

}

#if WITH_EDITOR
void ARuntimeSphereGenerator::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		if (PropertyChangedEvent.Property->GetFName() == FName("Radius") ||
			PropertyChangedEvent.Property->GetFName() == FName("Resolution"))
		{
			UpdateSphere();
		}
	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif