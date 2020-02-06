#pragma once

#include <SPlanner/AI/Tasks/Impl/SP_MoveToTask.h>
#include "SP_MoveToPOITask.generated.h"

/**
 *	MoveTo POI only spec implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|MoveTo")
class SPLANNER_API USP_MoveToPOITask : public USP_MoveToTask
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
};