// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_TaskChain.generated.h"

/**
 *	Chain of Tasks.
 *	Only the cooldown of TaskChain is used by planner.
 *	Cooldown of every tasks in Tasks are not used.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_TaskChain : public USP_Task
{
	GENERATED_BODY()

protected:
	/** The handled tasks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	TArray<USP_Task*> Tasks;

	void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos) override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_TaskInfos* InstantiateInfos() override;

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Steps). */
	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Steps). */
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;
};


/** Task info implementation for USP_TaskChain. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Action|Task")
class USP_TaskChainInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_TaskChain.
	friend USP_TaskChain;

	int Index = 0;

public:
	/**
	*	Current executed task infos.
	*	Must be UPROPERTY() to avoid garbage collection.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	USP_TaskInfos* TaskInfos = nullptr;
};