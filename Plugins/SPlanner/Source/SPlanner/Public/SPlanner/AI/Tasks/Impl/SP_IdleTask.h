// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_IdleTask.generated.h"

/**
 *	Idle task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_IdleTask : public USP_Task
{
	GENERATED_BODY()
	
protected:
	/** The minimum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MinTime = 0.5f;

	/** The maximum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MaxTime = 3.0f;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos) override;

public:
	USP_TaskInfosBase* InstantiateInfos() override;
};


/** Task info implementation for USP_IdleTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Action|Task")
class USP_IdleTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_IdleTask.
	friend USP_IdleTask;

	float WaitTime = -1.0f;
	float CurrTime = 0.0f;
};