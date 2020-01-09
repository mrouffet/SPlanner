#pragma once

#include <Actions/Tasks/Impl/SP_MoveToTask.h>
#include "SP_MoveToPOITask.generated.h"

/**
 *	MoveTo POI only spec implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|Task|MoveTo")
class SPLANNER_API UMoveToPOITask : public USP_MoveToTask
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;
};