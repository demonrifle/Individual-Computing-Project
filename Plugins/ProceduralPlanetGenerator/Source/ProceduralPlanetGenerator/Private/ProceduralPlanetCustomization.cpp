// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#include "ProceduralPlanetCustomization.h"
#include "PropertyEditing.h"
#include "ProceduralPlanetActor.h"

#define LOCTEXT_NAMESPACE "ProceduralPlanetGeneratorModule"

void FProceduralPlanetCustomization::CustomizeDetails(IDetailLayoutBuilder & DetailBuilder)
{
	DetailBuilder.HideCategory(TEXT("Materials"));
	DetailBuilder.HideCategory(TEXT("RuntimeMesh"));
	DetailBuilder.HideCategory(TEXT("Tags"));
	DetailBuilder.HideCategory(TEXT("Physics"));
	DetailBuilder.HideCategory(TEXT("Collision"));
	DetailBuilder.HideCategory(TEXT("Activation"));
	DetailBuilder.HideCategory(TEXT("Lighting"));
	DetailBuilder.HideCategory(TEXT("Cooking"));
	DetailBuilder.HideCategory(TEXT("Rendering"));
	DetailBuilder.HideCategory(TEXT("VirtualTexture"));
	DetailBuilder.HideCategory(TEXT("Replication"));
	DetailBuilder.HideCategory(TEXT("Actor"));
	DetailBuilder.HideCategory(TEXT("AssetUserData"));
	DetailBuilder.HideCategory(TEXT("MaterialParameters"));
	DetailBuilder.HideCategory(TEXT("HLOD"));
	DetailBuilder.HideCategory(TEXT("Mobile"));

	AProceduralPlanetActor* CustomizedPlanet = nullptr;
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			CustomizedPlanet = Cast<AProceduralPlanetActor>(Object);
			if (CustomizedPlanet)	break;
		}
	}

	check(CustomizedPlanet);

	IDetailCategoryBuilder& SeedCategory = DetailBuilder.EditCategory(TEXT("Planet Settings"));
	SeedCategory.AddCustomRow(LOCTEXT("MyWarningRowFilterString", "Procedural Planet"))
	.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Generate new Seed"))
		]
	.ValueContent()
		.VAlign(VAlign_Center)
		.MaxDesiredWidth(250)
		[
			SNew(SButton)
			.OnClicked_UObject(CustomizedPlanet, &AProceduralPlanetActor::Randomize)
			.VAlign(VAlign_Center)
		.ToolTipText(LOCTEXT("GenerateNewSeedTooltip", "Generates a new seed and randomizes all values from it"))
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Generate"))
		]
		];
}

TSharedRef<IDetailCustomization> FProceduralPlanetCustomization::MakeInstance()
{
	return MakeShareable(new FProceduralPlanetCustomization);
}

#undef LOCTEXT_NAMESPACE