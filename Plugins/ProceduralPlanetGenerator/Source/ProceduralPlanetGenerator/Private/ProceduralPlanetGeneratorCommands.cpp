// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProceduralPlanetGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FProceduralPlanetGeneratorModule"

void FProceduralPlanetGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(SpawnBlankPlanet, "Spawn Blank Planet", "Spawns a blank planet to edit", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SpawnRandomPlanet, "Spawn Random Planet", "Spawns a randomized planet to use or edit", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
