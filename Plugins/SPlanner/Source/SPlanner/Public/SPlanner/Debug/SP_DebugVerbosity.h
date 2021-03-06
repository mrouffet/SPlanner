// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_DebugVerbosity.generated.h"

UENUM(BlueprintType, Category = "SPlanner|Debug")
enum class ESP_DebugVerbosity : uint8
{
	/** Default display. */
	DV_Display			UMETA(DisplayName=Display),

	/** Orange color.*/
	DV_Warning			UMETA(DisplayName=Warning),

	/** Red color. */
	DV_Error			UMETA(DisplayName=Error),
};