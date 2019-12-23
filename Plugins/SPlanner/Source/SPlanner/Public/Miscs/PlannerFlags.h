#pragma once

#include "PlannerFlags.generated.h"

/** Flags depending on current plan generation. */
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Misc")
enum class ESP_PlannerFlags : uint8
{
	/** Target has changed. */
	PF_DirtyTarget			UMETA(Display = "DirtyTarget"),

	/** Transform has changed. */
	PF_DirtyTransform		UMETA(Display = "DirtyTransform"),


	/** The target will be a POI. */
	PF_POITarget			UMETA(Display = "POITarget"),
};

ENUM_CLASS_FLAGS(ESP_PlannerFlags)