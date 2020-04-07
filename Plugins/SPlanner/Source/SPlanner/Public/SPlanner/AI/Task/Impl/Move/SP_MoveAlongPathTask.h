// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/Move/SP_MoveBaseTask.h>
#include "SP_MoveAlongPathTask.generated.h"

/**
 *	Move along a SP_AIPath implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Move")
class SPLANNER_API USP_MoveAlongPathTask : public USP_MoveBaseTask
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bDisableMovementComponent = true;

	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Move")
	void SetMovementComponentEnabled(APawn* Pawn, bool bEnabled);

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_MoveAlongPathTask(const FObjectInitializer& ObjectInitializer);
};

/** Task info implementation for USP_MoveToTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Action|Task|Move")
class SPLANNER_API USP_MoveAlongPathTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_MoveAlongPathTask.
	friend USP_MoveAlongPathTask;

public:
	/** Sign of direction: either 1.0f or -1.0f. */
	float DirectionSign = 1.0f;

	/** Current input key in spline. */
	float InputKey = -1.0f;
};