#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_TargetNearestPlayerTask.generated.h"

/**
 *	Target nearest player task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_TargetNearestPlayerTask : public USP_Task
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};