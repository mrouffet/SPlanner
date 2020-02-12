// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/SP_FlagHelper.h>

#include "SP_PlannerFlags.generated.h"

/** Flags used during plan generation. */
UENUM(BlueprintType, Category = "SPlanner|Planner")
enum class ESP_PlannerFlags : uint8
{
	/**
	*	Max value of enum.
	*	Use this value to extend the enum.
	*/
	PF_Max					UMETA(Hidden),
};

ENUM_CLASS_FLAGS(ESP_PlannerFlags)