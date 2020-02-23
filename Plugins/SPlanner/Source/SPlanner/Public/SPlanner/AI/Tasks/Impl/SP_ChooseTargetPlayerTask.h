// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPlayerTask.generated.h"

/**
 *	Target nearest player task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetPlayerTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()

protected:
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;
};