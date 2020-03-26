// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskStep.h>
#include "SP_TaskChain.generated.h"

/**
 *	Chain of Tasks.
 *	Only the cooldown of TaskChain is used by planner.
 *	Cooldown of every tasks in Tasks are not used.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_TaskChain : public USP_TaskStep
{
	GENERATED_BODY()

protected:
	/** The handled tasks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	TArray<USP_TaskStep*> Tasks;

	bool IsAvailable(const USP_PlannerComponent* Planner) const override;

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Steps). */
	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Steps). */
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos) override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_TaskChain(const FObjectInitializer& ObjectInitializer);
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