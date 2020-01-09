#pragma once

#include <Actions/Tasks/SP_Task.h>
#include "SP_TargetNearestPlayerTask.generated.h"

/**
 *	Target nearest player task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|Task|Target")
class SPLANNER_API USP_TargetNearestPlayerTask : public USP_Task
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;
	FSP_PlannerFlags PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};