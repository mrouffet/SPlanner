// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPositionTask.generated.h"

/**
 *	Choose target implementation with position spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetPositionTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
protected:
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_ChooseTargetPositionTask(const FObjectInitializer& ObjectInitializer);
};