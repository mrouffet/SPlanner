// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_ParallelTask.generated.h"

/**
 *	Execute a 2 task simultaneously.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_ParallelTask : public USP_TaskImpl
{
	GENERATED_BODY()

protected:
	/**
	*	The main task to execute.
	*	This task end when main task end.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	USP_TaskImpl* MainTask = nullptr;

	/** The sub task to execute. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	USP_TaskImpl* SubTask = nullptr;

	/**
	*	Whether check sub conditions to execute MainTask.
	*	ie: SubTask must be available and valid to perform MainTask.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	bool bUseSubCheck = false;

	/**
	*	Whether to loop the sub task while waiting for MainTask end.
	*	Only loop on success.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	bool bLoopSubTask = false;

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
	USP_ParallelTask(const FObjectInitializer& ObjectInitializer);

	void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos) override;
};


/** Task info implementation for USP_ChainTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class USP_ParallelTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_ParallelTask.
	friend USP_ParallelTask;

public:
	/**
	*	Current main task infos.
	*	Must be UPROPERTY() to avoid garbage collection.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	USP_TaskInfos* MainTaskInfos = nullptr;

	/**
	*	Current sub task infos.
	*	Must be UPROPERTY() to avoid garbage collection.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner|Task|Parallel")
	USP_TaskInfos* SubTaskInfos = nullptr;
};