#pragma once

#include <SPlanner/Base/Planner/SP_PlannerFlags.h>
#include "SP_AIPlannerFlags.generated.h"

/** Extended flags used during plan generation for SP_AIPlannerComponent. */
UENUM(BlueprintType, Category = "SPlanner|Planner|AI")
enum class ESP_AIPlannerFlags : uint8
{
	/** Position has changed. */
	PF_LocationDirty	= ESP_PlannerFlags::PF_Max		UMETA(DisplayName = "LocationDirty"),

	/** Rotation has changed. */
	PF_RotationDirty									UMETA(DisplayName = "RotationDirty"),

	/** Target has changed. */
	PF_TargetDirty										UMETA(DisplayName = "TargetDirty"),

	/** The target will a Position. */
	PF_TargetPosition									UMETA(DisplayName = "TargetPosition"),

	/** The target will be an Actor. */
	PF_TargetActor										UMETA(DisplayName = "TargetActor"),

	/** The target will be a Player. */
	PF_TargetPlayer										UMETA(DisplayName = "TargetPlayer"),

	/** The target will be a POI. */
	PF_TargetPOI										UMETA(DisplayName = "TargetPOI"),

	/**
	*	Max value of enum.
	*	Use this value to extend the enum.
	*/
	PF_Max												UMETA(Hidden),
};

ENUM_CLASS_FLAGS(ESP_AIPlannerFlags)