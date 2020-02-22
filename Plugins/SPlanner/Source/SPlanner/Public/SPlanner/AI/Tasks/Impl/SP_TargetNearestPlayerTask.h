// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_TargetNearestPlayerTask.generated.h"

/**
 *	Target nearest player task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_TargetNearestPlayerTask : public USP_Task
{
	GENERATED_BODY()

protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName TargetEntryName = "None";

	/**
	*	Predicate to validate to choose a player.
	*	Can be overridden in children, otherwise return true.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|Target")
	bool Predicate(USP_AIPlannerComponent* Planner, APawn* Player) const;

	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfosBase* TaskInfos) override;

public:
	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;
};