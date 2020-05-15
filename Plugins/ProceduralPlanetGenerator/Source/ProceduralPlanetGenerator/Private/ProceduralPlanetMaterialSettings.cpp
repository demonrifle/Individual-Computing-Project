// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetMaterialSettings.h"

UProceduralPlanetMaterialSettings::UProceduralPlanetMaterialSettings()
	:SphereMaterial(nullptr),
	Texture1(nullptr),
	Texture2(nullptr),
	Texture3(nullptr),
	Texture4(nullptr),
	Texture1Height(0.f),
	Texture2Height(0.f),
	Texture3Height(0.f),
	Texture4Height(0.f),
	Texture1Tiling(FVector2D(0.f,0.f)),
	Texture2Tiling(FVector2D(0.f, 0.f)),
	Texture3Tiling(FVector2D(0.f, 0.f)),
	Texture4Tiling(FVector2D(0.f, 0.f))
{
}

void UProceduralPlanetMaterialSettings::Initialize()
{
	UMaterial* PlanetMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/ProceduralPlanetMaterial"));
	Texture1 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Water_D"));
	Texture2 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Sand_D"));
	Texture3 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Ground_Grass_D"));
	Texture4 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Snow_D"));
	MaterialInstance = UMaterialInstanceDynamic::Create(PlanetMaterial, this);
	Update();
	SphereMaterial = MaterialInstance;
}

void UProceduralPlanetMaterialSettings::Update()
{
	MaterialInstance->SetTextureParameterValue(FName("Texture1"), Texture1);
	MaterialInstance->SetTextureParameterValue(FName("Texture2"), Texture2);
	MaterialInstance->SetTextureParameterValue(FName("Texture3"), Texture3);
	MaterialInstance->SetTextureParameterValue(FName("Texture4"), Texture4);
	MaterialInstance->SetScalarParameterValue(FName("Texture1UTiling"), Texture1Tiling.X);
	MaterialInstance->SetScalarParameterValue(FName("Texture1VTiling"), Texture1Tiling.Y);
	MaterialInstance->SetScalarParameterValue(FName("Texture2UTiling"), Texture2Tiling.X);
	MaterialInstance->SetScalarParameterValue(FName("Texture2VTiling"), Texture2Tiling.Y);
	MaterialInstance->SetScalarParameterValue(FName("Texture3UTiling"), Texture3Tiling.X);
	MaterialInstance->SetScalarParameterValue(FName("Texture3VTiling"), Texture3Tiling.Y);
	MaterialInstance->SetScalarParameterValue(FName("Texture4UTiling"), Texture4Tiling.X);
	MaterialInstance->SetScalarParameterValue(FName("Texture4VTiling"), Texture4Tiling.Y);
}


void UProceduralPlanetMaterialSettings::Randomize(FRandomStream* Seed)
{
	bool ShouldBeFullyTextured = Seed->RandRange(0, 1);
	if (ShouldBeFullyTextured)
	{
		Texture1Height = Seed->FRandRange(0.f, 0.2f);
		Texture2Height = Seed->FRandRange(Texture1Height, 0.5f);
		Texture3Height = Seed->FRandRange(Texture2Height, 0.8f);
		Texture4Height = Seed->FRandRange(Texture3Height, 1.f);
	}
	else
	{
		Texture1Height = Seed->FRandRange(0.f, 1.0f);
		Texture2Height = Seed->FRandRange(0.f, 1.0f);
		Texture3Height = Seed->FRandRange(0.f, 1.0f);
		Texture4Height = Seed->FRandRange(0.f, 1.0f);
	}
	Texture1Tiling.X = Seed->FRandRange(1.0f, 5.0f);
	Texture1Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture2Tiling.X = Seed->FRandRange(1.0f, 5.0f);
	Texture2Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture3Tiling.X = Seed->FRandRange(1.0f, 5.0f);
	Texture3Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture4Tiling.X = Seed->FRandRange(1.0f, 5.0f);
	Texture4Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Update();
}

FColor UProceduralPlanetMaterialSettings::GetVertexColorFor3DHeight(float Height, float MaxHeight)
{
	float VertexScale = Height / MaxHeight;
	FColor VertexColor;
	uint8 VisibleTexture = 255;
	if (VertexScale <= Texture1Height)
	{
		VertexColor = FColor(0, 0, 0, 0);
	}
	else if (VertexScale <= Texture2Height)
	{

		VertexColor = FColor(VisibleTexture, 0, 0, 0);
	}
	else if (VertexScale <= Texture3Height)
	{

		VertexColor = FColor(0, VisibleTexture, 0, 0);
	}
	else if (VertexScale <= Texture4Height)
	{

		VertexColor = FColor(0, 0, VisibleTexture, 0);
	}
	return VertexColor;
}


#if WITH_EDITOR
void UProceduralPlanetMaterialSettings::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		Update();
	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif