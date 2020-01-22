#pragma once

#include <SPlanner/Miscs/Flags/SP_PlannerFlags.h>
#include "SP_AIPlannerFlags.generated.h"

/** Extended flags used during plan generation for SP_AIPlannerComponent. */
UENUM(BlueprintType, Category = "SPlanner|Misc|Flags")
enum class ESP_AIPlannerFlags : uint8
{
	/** Position has changed. */
	PF_LocationDirty	= ESP_PlannerFlags::PF_Max		UMETA(Display = "LocationDirty"),

	/** Rotation has changed. */
	PF_RotationDirty									UMETA(Display = "RotationDirty"),

	/** Target has changed. */
	PF_TargetDirty										UMETA(Display = "TargetDirty"),

	/** The target will a Position. */
	PF_TargetPosition									UMETA(Display = "TargetPosition"),

	/** The target will be an Actor. */
	PF_TargetActor										UMETA(Display = "TargetActor"),

	/** The target will be a Player. */
	PF_TargetPlayer										UMETA(Display = "TargetPlayer"),

	/** The target will be a POI. */
	PF_TargetPOI										UMETA(Display = "TargetPOI"),

	/**
	*	Max value of enum.
	*	Use this value to extend the enum.
	*/
	PF_Max					UMETA(Hidden),
};

ENUM_CLASS_FLAGS(ESP_AIPlannerFlags)