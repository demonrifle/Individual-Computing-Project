// Internal code - Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Custom code - Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#include "ProceduralPlanetGenerator.h"
#include "ProceduralPlanetGeneratorStyle.h"
#include "ProceduralPlanetGeneratorCommands.h"
#include "ProceduralPlanetCustomization.h"
#include "ProceduralPlanetActor.h"
#include "Misc/MessageDialog.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor/EditorEngine.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"

#include "LevelEditor.h"

static const FName ProceduralPlanetGeneratorTabName("ProceduralPlanetGenerator");

#define LOCTEXT_NAMESPACE "FProceduralPlanetGeneratorModule"

void FProceduralPlanetGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Initializes any UI for this module
	FProceduralPlanetGeneratorStyle::Initialize();
	FProceduralPlanetGeneratorStyle::ReloadTextures();

	// Registers declared commands 
	FProceduralPlanetGeneratorCommands::Register();
	
	// Initialize and map all actions
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet,
		FExecuteAction::CreateRaw(this, &FProceduralPlanetGeneratorModule::SpawnBlankPlanet),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet,
		FExecuteAction::CreateRaw(this, &FProceduralPlanetGeneratorModule::SpawnRandomPlanet),
		FCanExecuteAction());

		// Get the level editor module
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	// Initialize a new MenuExtender
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

	// Add the menu Extension
	MenuExtender->AddMenuExtension(
		// Extension hook to add around
		"WindowLayout", 
		// Place to add
		EExtensionHook::After, 
		PluginCommands, 
		// Bind delegate to execute
		FMenuExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddMenuExtension));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Initialize a new ToolbarExtender
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	// Add/register extension to it
	ToolbarExtender->AddToolBarExtension(
		//Extension hook to add around
		"Game",
		// Place to add
		EExtensionHook::After, 
		PluginCommands,
		// Bind delegate to execute
		FToolBarExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddToolbarExtension));
		
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);


	// Get property editor module
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule >("PropertyEditor");

	// Register NoiseLayer to be customized by the NoiseLayerCustomization class
	PropertyEditor.RegisterCustomClassLayout
	(
		"ProceduralPlanetActor",
		FOnGetDetailCustomizationInstance::CreateStatic(&FProceduralPlanetCustomization::MakeInstance)
	);

	//Notify customization
	PropertyEditor.NotifyCustomizationModuleChanged();
	
}

void FProceduralPlanetGeneratorModule::ShutdownModule()
{	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FProceduralPlanetGeneratorStyle::Shutdown();

	FProceduralPlanetGeneratorCommands::Unregister();

	//Unregister class customization on shutdown
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		PropertyEditor.UnregisterCustomClassLayout("NoiseLayer");
	}
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

		// Finish spawning at position
		UGameplayStatics::FinishSpawningActor(ProceduralPlanetActor, FTransform::Identity);

		// Calculate view bounds of the new generation and focus the camera on it
		//FBox ViewBounds = ProceduralPlanetActor->PlanetProvider->GetBounds().GetBox();
		//GEditor->MoveViewportCamerasToBox(ViewBounds, true);
		// Set new actor as selection
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(ProceduralPlanetActor, true, false, false, true);
	}


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

		// Finish spawning at position
		UGameplayStatics::FinishSpawningActor(ProceduralPlanetActor, FTransform::Identity);

		// Calculate view bounds of the new generation and focus the camera on it
		//FBox ViewBounds = ProceduralPlanetActor->PlanetProvider->GetBounds().GetBox();
		G//Editor->MoveViewportCamerasToBox(ViewBounds, true);
		// Set new actor as selection
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(ProceduralPlanetActor, true, false, false, true);
	}

}


void FProceduralPlanetGeneratorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	// UI Elements to add
	Builder.BeginSection("PlanetEngine");
	{
		Builder.AddMenuEntry(FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet);
		Builder.AddMenuEntry(FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet);
	}
	Builder.EndSection();
}

void FProceduralPlanetGeneratorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	// UI Elements to add
	Builder.BeginSection("PlanetEngine");
	{
		Builder.AddToolBarButton(FProceduralPlanetGeneratorCommands::Get().SpawnBlankPlanet);
		Builder.AddToolBarButton(FProceduralPlanetGeneratorCommands::Get().SpawnRandomPlanet);
	}
	Builder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProceduralPlanetGeneratorModule, ProceduralPlanetGenerator)