// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralPlanetGenerator.h"
#include "ProceduralPlanetGeneratorStyle.h"
#include "ProceduralPlanetGeneratorCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

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
		FProceduralPlanetGeneratorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FProceduralPlanetGeneratorModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FProceduralPlanetGeneratorModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FProceduralPlanetGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FProceduralPlanetGeneratorStyle::Shutdown();

	FProceduralPlanetGeneratorCommands::Unregister();
}

void FProceduralPlanetGeneratorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FProceduralPlanetGeneratorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("ProceduralPlanetGenerator.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FProceduralPlanetGeneratorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FProceduralPlanetGeneratorCommands::Get().PluginAction);
}

void FProceduralPlanetGeneratorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FProceduralPlanetGeneratorCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProceduralPlanetGeneratorModule, ProceduralPlanetGenerator)