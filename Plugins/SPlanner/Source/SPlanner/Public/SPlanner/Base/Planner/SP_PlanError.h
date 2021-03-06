// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_PlanError.generated.h"

/** Error list generated by SPlanner. */
UENUM(BlueprintType, Category = "SPlanner|Planner")
enum class ESP_PlanError : uint8
{
	/** Unknown error. */
	PE_Unknown								UMETA(DisplayName = "Unknown"),

	/** Bad argument provided for plan generation. */
	PE_BadArgument							UMETA(DisplayName = "BadArgument"),

	/** Goal can't be achieved during generation. */
	PE_CantBeAchieved						UMETA(DisplayName = "CanBeAchieved"),

	/** LOD get out of range. */
	PE_LODOutOfRange						UMETA(DisplayName = "LODOutOfRange"),
};