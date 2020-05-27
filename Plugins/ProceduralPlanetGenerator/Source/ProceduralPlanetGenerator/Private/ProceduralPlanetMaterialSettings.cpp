// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "ProceduralPlanetMaterialSettings.h"

UProceduralPlanetMaterialSettings::UProceduralPlanetMaterialSettings()
	:SphereMaterial(nullptr),

	Texture1(nullptr),
	Texture1Height(0.25f),
	Texture1Tiling(FVector2D(1.f, 1.f)),

	Texture2(nullptr),
	Texture2Height(0.50f),
	Texture2Tiling(FVector2D(1.f, 1.f)),

	Texture3(nullptr),
	Texture3Height(0.75f),
	Texture3Tiling(FVector2D(1.f, 1.f)),

	Texture4(nullptr),
	Texture4Height(1.00f),
	Texture4Tiling(FVector2D(1.f, 1.f))
{
	DefaultMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EngineMaterials/DefaultMaterial"));
	DefaultTexture = LoadObject<UTexture>(nullptr, TEXT("/Engine/EngineResources/WhiteSquareTexture"));
	SphereMaterial = DefaultMaterial;
	Texture1 = Texture2 = Texture3 = Texture4 = DefaultTexture;


	PlanetMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_ProceduralPlanetMaterial"));
	Texture1 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Water_D"));
	Texture2 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Sand_D"));
	Texture3 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Ground_Grass_D"));
	Texture4 = LoadObject<UTexture>(nullptr, TEXT("/Game/StarterContent/Textures/T_Snow_D"));
}

void UProceduralPlanetMaterialSettings::Initialize()
{	
	MaterialInstance = UMaterialInstanceDynamic::Create(PlanetMaterial, this);
	
	Update();
	SphereMaterial = MaterialInstance;
}

void UProceduralPlanetMaterialSettings::Update()
{
	if (MaterialInstance)
	{
		if (Texture1)	MaterialInstance->SetTextureParameterValue(FName("Texture1"), Texture1);
		else MaterialInstance->SetTextureParameterValue(FName("Texture1"), DefaultTexture);
		if (Texture2)	MaterialInstance->SetTextureParameterValue(FName("Texture2"), Texture2);
		else MaterialInstance->SetTextureParameterValue(FName("Texture2"), DefaultTexture);
		if (Texture3)	MaterialInstance->SetTextureParameterValue(FName("Texture3"), Texture3);
		else MaterialInstance->SetTextureParameterValue(FName("Texture3"), DefaultTexture);
		if (Texture4)	MaterialInstance->SetTextureParameterValue(FName("Texture4"), Texture4);
		else MaterialInstance->SetTextureParameterValue(FName("Texture4"), DefaultTexture);

		MaterialInstance->SetScalarParameterValue(FName("Texture1UTiling"), Texture1Tiling.X);
		MaterialInstance->SetScalarParameterValue(FName("Texture1VTiling"), Texture1Tiling.Y);
		MaterialInstance->SetScalarParameterValue(FName("Texture2UTiling"), Texture2Tiling.X);
		MaterialInstance->SetScalarParameterValue(FName("Texture2VTiling"), Texture2Tiling.Y);
		MaterialInstance->SetScalarParameterValue(FName("Texture3UTiling"), Texture3Tiling.X);
		MaterialInstance->SetScalarParameterValue(FName("Texture3VTiling"), Texture3Tiling.Y);
		MaterialInstance->SetScalarParameterValue(FName("Texture4UTiling"), Texture4Tiling.X);
		MaterialInstance->SetScalarParameterValue(FName("Texture4VTiling"), Texture4Tiling.Y);
	}
}


void UProceduralPlanetMaterialSettings::Randomize(FRandomStream* Seed)
{
	bool ShouldBeFullyTextured = (bool) Seed->RandRange(0, 1);
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
	Texture1Tiling.X = Texture1Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture2Tiling.X = Texture2Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture3Tiling.X = Texture3Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
	Texture4Tiling.X = Texture4Tiling.Y = Seed->FRandRange(1.0f, 5.0f);
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
	else VertexColor = GetVertexColorForHighestTexture();
	return VertexColor;
}

FColor UProceduralPlanetMaterialSettings::GetVertexColorForHighestTexture()
{
	FColor VertexColor;
	//Get Highest Texture Value
	float Max = FMath::Max(Texture1Height,
					FMath::Max(Texture2Height,
						FMath::Max(Texture3Height,Texture3Height)));

	Max == Texture1Height ? VertexColor = FColor(0, 0, 0, 0) :
		Max == Texture2Height ? VertexColor = FColor(255, 0, 0, 0) :
			Max == Texture3Height ? VertexColor = FColor(0, 255, 0, 0) :
										VertexColor = FColor(0, 0, 255, 0);
	return VertexColor;
}

#if WITH_EDITOR
void UProceduralPlanetMaterialSettings::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		Update();
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == "SphereMaterial")
		{
			if (SphereMaterial)
			{
				if (SphereMaterial->GetMaterial()->GetName() == PlanetMaterial->GetName())
				{
					Initialize();
				}
			}
			else
			{
				MaterialInstance = UMaterialInstanceDynamic::Create(PlanetMaterial, this);
				SphereMaterial = MaterialInstance;
			}
		}
	}

	//Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif