// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralPlanetGenerator.h"
#include "ProceduralPlanetGeneratorStyle.h"
#include "ProceduralPlanetGeneratorCommands.h"
#include "ProceduralPlanetActor.h"
#include "Misc/MessageDialog.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor/EditorEngine.h"

#include "LevelEditor.h"

static const FName ProceduralPlanetGeneratorTabName("ProceduralPlanetGenerator");

#define LOCTEXT_NAMESPACE "FProceduralPlanetGeneratorModule"

void FProceduralPlanetGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FProceduralPlanetGeneratorStyle::Initialize();
	FProceduralPlanetGeneratorStyle::ReloadTextures();

	FProceduralPlanetGeneratorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet,
		FExecuteAction::CreateRaw(this, &FProceduralPlanetGeneratorModule::SpawnBlankPlanet),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet,
		FExecuteAction::CreateRaw(this, &FProceduralPlanetGeneratorModule::SpawnRandomPlanet),
		FCanExecuteAction());

		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddMenuExtension));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Make new ToolbarExtender
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	// Add/register extension to it
	ToolbarExtender->AddToolBarExtension(
		//After Game Section
		"Game", 
		EExtensionHook::After, 
		PluginCommands, 
		FToolBarExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddToolbarExtension));
		
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	
}

void FProceduralPlanetGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FProceduralPlanetGeneratorStyle::Shutdown();

	FProceduralPlanetGeneratorCommands::Unregister();
}

void FProceduralPlanetGeneratorModule::SpawnBlankPlanet()
{
	// Spawns a deffered actor which allows initializing values
	AProceduralPlanetActor* ProceduralPlanetActor = GEditor->GetEditorWorldContext().World()->SpawnActorDeferred<AProceduralPlanetActor>(
		AProceduralPlanetActor::StaticClass(), //Class to spawn
		FTransform::Identity, // Place to spawn at
		nullptr, // Owner object
		nullptr, // Instigator, in this case always null
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn); // Whether object should be spawn if it collides with objects on spawn

	// Validate
	if (ProceduralPlanetActor)
	{
		// Initialize with blank variables
		ProceduralPlanetActor->Initialize(false);
	}

	// Finish spawning at position
	UGameplayStatics::FinishSpawningActor(ProceduralPlanetActor, FTransform::Identity);

	// Calculate view bounds of the new generation and focus the camera on it
	FBox ViewBounds = ProceduralPlanetActor->PlanetProvider->GetBounds().GetBox();
	GEditor->MoveViewportCamerasToBox(ViewBounds, true);

}

void FProceduralPlanetGeneratorModule::SpawnRandomPlanet()
{	
	// Spawns a deffered actor which allows initializing values
	AProceduralPlanetActor* ProceduralPlanetActor = GEditor->GetEditorWorldContext().World()->SpawnActorDeferred<AProceduralPlanetActor>(
		AProceduralPlanetActor::StaticClass(), //Class to spawn
		FTransform::Identity, // Place to spawn at
		nullptr, // Owner object
		nullptr, // Instigator, in this case always null
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn); // Whether object should be spawn if it collides with objects on spawn

	// Validate
	if (ProceduralPlanetActor)
	{
		// Initialize with blank variables
		ProceduralPlanetActor->Initialize(true);
	}

	// Finish spawning at position
	UGameplayStatics::FinishSpawningActor(ProceduralPlanetActor, FTransform::Identity);

	// Calculate view bounds of the new generation and focus the camera on it
	FBox ViewBounds = ProceduralPlanetActor->PlanetProvider->GetBounds().GetBox();
	GEditor->MoveViewportCamerasToBox(ViewBounds, true);
}


void FProceduralPlanetGeneratorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.BeginSection("PlanetEngine");
	{
		Builder.AddMenuEntry(FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet);
		Builder.AddMenuEntry(FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet);
	}
	Builder.EndSection();
}

void FProceduralPlanetGeneratorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.BeginSection("PlanetEngine");
	{
		Builder.AddToolBarButton(FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet);
		Builder.AddToolBarButton(FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet);
	}
	Builder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProceduralPlanetGeneratorModule, ProceduralPlanetGenerator)