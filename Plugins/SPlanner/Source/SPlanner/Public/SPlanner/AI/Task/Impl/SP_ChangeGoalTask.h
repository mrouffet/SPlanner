// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskStep.h>
#include "SP_ChangeGoalTask.generated.h"

class USP_Goal;

/**
 *	Task to change planner goal.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Goal")
class SPLANNER_API USP_ChangeGoalTask : public USP_TaskStep
{
	GENERATED_BODY()
	
protected:
	/** The new Goal to set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* NewGoal = nullptr;

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
};