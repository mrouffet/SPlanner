#pragma once

#include "SP_ActionGenerationType.generated.h"

/** Generation type for a given action. */
UENUM(BlueprintType, Category = "SPlanner|Action")
enum class ESP_ActionGenerationType : uint8
{
	/** The action can only be added once in plan. */
	AGT_OnceInPlan							UMETA(DisplayName = "OnceInPlan"),

	/** Push back this action to the generation set after being add to the plan. */
	AGT_PushBack							UMETA(DisplayName = "PushBack"),

	/** Generate a new weight for this action and add it to the generation set after being add to the plan. */
	AGT_RandomWeight						UMETA(DisplayName = "RandomWeight"),
};