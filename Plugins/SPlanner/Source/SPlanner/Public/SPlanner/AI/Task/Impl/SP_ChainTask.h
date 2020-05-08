// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_ChainTask.generated.h"

/**
 *	Chain of Tasks.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_ChainTask : public USP_TaskImpl
{
	GENERATED_BODY()

protected:
	/** The handled tasks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	TArray<USP_TaskImpl*> Tasks;

	bool IsAvailable(const USP_PlannerComponent* Planner) const override;
	void PostExecution(const USP_PlannerComponent* Planner, bool bExecutionSuccess) override;

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Impls). */
	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Impls). */
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_ChainTask(const FObjectInitializer& ObjectInitializer);

	void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos) override;
};


/** Task info implementation for USP_ChainTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class USP_ChainTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_ChainTask.
	friend USP_ChainTask;

	int Index = 0;

	bool bSubHasStarted = false;

public:
	/**
	*	Current executed task infos.
	*	Must be UPROPERTY() to avoid garbage collection.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	USP_TaskInfos* TaskInfos = nullptr;
};