#pragma once

#include <SPlanner/Miscs/Flags/SP_FlagHelper.h>

#include "SP_PlannerFlags.generated.h"

/** Flags used during plan generation. */
UENUM(BlueprintType, Category = "SPlanner|Misc|Flags")
enum class ESP_PlannerFlags : uint8
{
	/**
	*	Max value of enum.
	*	Use this value to extend the enum.
	*/
	PF_Max					UMETA(Hidden),
};

ENUM_CLASS_FLAGS(ESP_PlannerFlags)