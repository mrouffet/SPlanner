// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_IdleBaseTask.generated.h"

/**
 *	Idle base task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_IdleBaseTask : public USP_TaskImpl
{
	GENERATED_BODY()
	
protected:
	/** Generate the time for the task. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Task|Idle")
	float GenerateTime(const USP_AIPlannerComponent* Planner);

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_IdleBaseTask(const FObjectInitializer& ObjectInitializer);
};


/** Task info implementation for USP_IdleTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_IdleBaseTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_IdleTask.
	friend USP_IdleBaseTask;

	float WaitTime = -1.0f;
	float CurrTime = 0.0f;
};