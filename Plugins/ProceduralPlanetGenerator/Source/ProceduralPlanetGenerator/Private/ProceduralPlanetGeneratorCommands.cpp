// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralPlanetGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FProceduralPlanetGeneratorModule"

void FProceduralPlanetGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ProceduralPlanetGenerator", "Execute ProceduralPlanetGenerator action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
