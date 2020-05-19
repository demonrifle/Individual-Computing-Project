// Internal code - Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Custom code - Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#include "NoiseGenerator.h"
#include "NoiseLayerCustomization.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"

static const FName NoiseGeneratorTabName("NoiseGenerator");

DEFINE_LOG_CATEGORY(NoiseGeneratorModule);

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