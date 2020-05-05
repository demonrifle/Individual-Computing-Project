// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NoiseGenerator.h"
#include "NoiseLayerCustomization.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"

static const FName NoiseGeneratorTabName("NoiseGenerator");

#define LOCTEXT_NAMESPACE "FNoiseGeneratorModule"

void FNoiseGeneratorModule::StartupModule()
{
	// Get property editor module
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule >("PropertyEditor");
	
	// Register NoiseLayer to be customized by the NoiseLayerCustomization class
	PropertyEditor.RegisterCustomClassLayout
	(
		"NoiseLayer",
		FOnGetDetailCustomizationInstance::CreateStatic(&FNoiseLayerCustomization::MakeInstance)
	);

	//Notify customization
	PropertyEditor.NotifyCustomizationModuleChanged();
}

void FNoiseGeneratorModule::ShutdownModule()
{
	//Unregister class customization on shutdown
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		PropertyEditor.UnregisterCustomClassLayout("NoiseLayer");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNoiseGeneratorModule, NoiseGenerator)