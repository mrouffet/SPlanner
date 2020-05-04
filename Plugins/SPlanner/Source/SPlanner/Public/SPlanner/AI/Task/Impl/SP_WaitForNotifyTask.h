// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_WaitForNotifyTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|Task|Notify")
class SPLANNER_API USP_WaitForNotifyTask : public USP_TaskImpl
{
	GENERATED_BODY()
	
protected:
	/** Current pending infos. */
	TArray<USP_WaitForNotifyTaskInfos*> CachedInfos;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_WaitForNotifyTask(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Notify")
	void Notify();
};

/** Task info implementation for USP_TaskImpl. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_WaitForNotifyTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_TaskImpl.
	friend USP_TaskImpl;

public:
	ESP_PlanExecutionState NotifyExecutionState = ESP_PlanExecutionState::PES_Running;
};