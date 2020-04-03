// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskStep.h>
#include "SP_WaitForBlackboardValueTask.generated.h"

class USP_VariableParam;

/**
*	Wait for value in blackboard.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_WaitForBlackboardValueTask : public USP_TaskStep
{
	GENERATED_BODY()

protected:
	/** The entry name to access object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|SetValue")
	FName EntryName = "None";

	/** Value to compare in blackboard. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner|Task|SetValue")
	USP_VariableParam* Param;

	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
};