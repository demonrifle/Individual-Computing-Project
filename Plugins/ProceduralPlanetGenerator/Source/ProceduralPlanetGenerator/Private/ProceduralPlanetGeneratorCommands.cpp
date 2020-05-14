// Internal code - Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Custom code - Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#include "ProceduralPlanetGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FProceduralPlanetGeneratorModule"

void FProceduralPlanetGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(SpawnBlankPlanet, "Spawn Blank Planet", "Spawns a blank planet to edit", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SpawnRandomPlanet, "Spawn Random Planet", "Spawns a randomized planet to use or edit", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
