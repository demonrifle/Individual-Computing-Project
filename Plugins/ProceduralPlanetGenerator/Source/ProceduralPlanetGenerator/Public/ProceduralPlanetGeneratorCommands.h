// Internal code - Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Custom code - Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ProceduralPlanetGeneratorStyle.h"

class FProceduralPlanetGeneratorCommands : public TCommands<FProceduralPlanetGeneratorCommands>
{
public:

	FProceduralPlanetGeneratorCommands()
		: TCommands<FProceduralPlanetGeneratorCommands>(
			TEXT("ProceduralPlanetGenerator"),
			NSLOCTEXT("Contexts", "ProceduralPlanetGenerator", "ProceduralPlanetGenerator Plugin"),
			NAME_None, 
			FProceduralPlanetGeneratorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > SpawnBlankPlanet;
	TSharedPtr< FUICommandInfo > SpawnRandomPlanet;
};
