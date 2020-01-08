#pragma once

#include <Miscs/Flags/FlagHelper.h>
#include <Miscs/Flags/TransformFlags.h>
#include <Miscs/Flags/TargetFlags.h>

#include "PlannerFlags.generated.h"

/** Flags depending on current plan generation. */
USTRUCT(BlueprintType, Category = "SPlanner|Misc|Flags")
struct FSP_PlannerFlags
{
	GENERATED_BODY()

public:
	/** Flags depending on transform. */
	UPROPERTY(BlueprintReadWrite, Category = "SPlanner")
	int TransformFlags = 0;

	/** Flags depending on target. */
	UPROPERTY(BlueprintReadWrite, Category = "SPlanner")
	int TargetFlags = 0;
};