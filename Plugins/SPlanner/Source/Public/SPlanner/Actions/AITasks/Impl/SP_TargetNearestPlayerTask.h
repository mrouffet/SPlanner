#pragma once

#include <SPlanner/Actions/AITasks/SP_AITask.h>
#include "SP_TargetNearestPlayerTask.generated.h"

/**
 *	Target nearest player task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|AITask|Target")
class SPLANNER_API USP_TargetNearestPlayerTask : public USP_AITask
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};