// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralPlanetGeneratorStyle.h"
#include "ProceduralPlanetGenerator.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FProceduralPlanetGeneratorStyle::StyleInstance = NULL;

void FProceduralPlanetGeneratorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FProceduralPlanetGeneratorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FProceduralPlanetGeneratorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ProceduralPlanetGeneratorStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon256x256(256.0f, 256.0f);

TSharedRef< FSlateStyleSet > FProceduralPlanetGeneratorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ProceduralPlanetGeneratorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ProceduralPlanetGenerator")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ProceduralPlanetGenerator.SpawnBlankPlanet", new IMAGE_BRUSH(TEXT("BlankPlanetIcon_256x"), Icon40x40));
	Style->Set("ProceduralPlanetGenerator.SpawnRandomPlanet", new IMAGE_BRUSH(TEXT("BlankPlanetIcon_256x"), Icon40x40));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FProceduralPlanetGeneratorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FProceduralPlanetGeneratorStyle::Get()
{
	return *StyleInstance;
}
