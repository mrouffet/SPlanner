#pragma once

#include "SP_TargetFlags.generated.h"

/** Flags used during plan generation for Transform. */
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Misc|Flags")
enum class ESP_TargetFlags : uint8
{
	/** Target has changed. */
	TF_Dirty				UMETA(Display = "Dirty"),

	/** The target will a Position. */
	TF_Position				UMETA(Display = "Position"),

	/** The target will be an Actor. */
	TF_Actor				UMETA(Display = "Actor"),

	/** The target will be a Player. */
	TF_Player				UMETA(Display = "Player"),

	/** The target will be a POI. */
	TF_POI					UMETA(Display = "POI"),
};

ENUM_CLASS_FLAGS(ESP_TargetFlags)