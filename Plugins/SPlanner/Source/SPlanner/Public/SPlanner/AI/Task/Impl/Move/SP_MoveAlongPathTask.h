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
	/** Whether movement component should be disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bUseMovementComponent = true;

	/**
	* The direction to apply to the spline.
	* Set 1.0f or -1.0f for fixed direction.
	* Set 0.0f for random direction.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	float DirectionSign = 0.0f;

	/**
	* Whether movement should follow the spline loop.
	* Require Spline.ClosedLoop == true.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bLoopSpline = true;

	/** Apply the computed location (world space). */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Move")
	void ApplyLocation(APawn* Pawn, const FVector& NewLocation);

	/** Apply the computed rotation (world space). */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Move")
	void ApplyRotation(APawn* Pawn, const FRotator& NewRotation);

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_MoveAlongPathTask(const FObjectInitializer& ObjectInitializer);
};

/** Task info implementation for USP_MoveToTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task|Move")
class SPLANNER_API USP_MoveAlongPathTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_MoveAlongPathTask.
	friend USP_MoveAlongPathTask;

public:
	/** Sign of direction: either 1.0f or -1.0f. */
	float DirectionSign = 1.0f;

	/** Current input key in spline. */
	float StartInputKey = -1.0f;

	/** Current input key in spline. */
	float InputKey = -1.0f;

	bool bHasLooped = false;
};