// Internal code - Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Custom code - Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(ProceduralPlanetModule, Verbose, All);

class FToolBarBuilder;
class FMenuBuilder;

// Procedural Planet Generator Module object
class FProceduralPlanetGeneratorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** These functions will be bound to Command. */
	void SpawnBlankPlanet();
	void SpawnRandomPlanet();
	
private:

	// Add all desired interface for extending the editor inside
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
