// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_AIPlannerNotify.generated.h"

/**
*	AI Planner notify enum.
*/
UENUM(BlueprintType, Meta = (Bitflags), Category = "SPlanner|AI")
enum class ESP_AIPlannerNotify : uint8
{
	/** Notify when joined formation change. */
	NP_FormationChange			UMETA(DisplayName = "FormationChange"),
};

ENUM_CLASS_FLAGS(ESP_AIPlannerNotify);