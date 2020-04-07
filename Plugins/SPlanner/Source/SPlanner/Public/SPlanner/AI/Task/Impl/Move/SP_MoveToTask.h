// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <AIController.h>

#include <SPlanner/AI/Task/Impl/Move/SP_MoveBaseTask.h>
#include "SP_MoveToTask.generated.h"

class USP_Target;
class USP_MoveToTaskInfos;

/**
 *	MoveTo implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Move")
class SPLANNER_API USP_MoveToTask : public USP_MoveBaseTask
{
	GENERATED_BODY()
	
protected:
	/**
	*	Map of RequestID, task infos.
	*	Used to get back task infos on move completed.
	*/
	TMap<int, USP_MoveToTaskInfos*> RequestIDToTaskInfos;

	/** Should use pathfinding. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bUsePathfinding = true;

	/** Whether can strafe during move to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bCanStrafe = false;

	/** Should stop movement while task get cancelled. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bStopMovementOnCancel = true;

	/** The radius to accept the move to as completed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	float AcceptanceRadius = 10.0f;

	/** Should re-compute MoveTo on tick (only used when Target is not an actor). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bIsDynamic = true;

	/**
	*	The frequency of update while bIsDynamic it true.
	*	Use < 0.0f for every tick.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move", meta=(EditCondition="bIsDynamic"))
	float DynamicUpdateFrequency = -1.0f;

	/**
	*	Whether precondition should fail if Pawn is already at goal.
	*	Mostly use true, should use false when using Formation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bPreconditionFailWhileAlreadyAtGoal = true;

	/** Child implementation to set pawn speed during MoveTo. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|MoveTo")
	void SetPawnSpeed(APawn* Pawn, float NewPawnSpeed);

	/** Check if owner actor has reached its target. */
	bool HasReachedPosition(const USP_AIPlannerComponent* Planner, const USP_Target* Target) const;

	/**
	*	Check if owner actor has reached its target.
	*	Internal implementation.
	*/
	bool HasReachedPosition(APawn* Pawn, const FVector& TargetPosition) const;

	/**
	*	Check if owner character has reached its target.
	*	Internal implementation with capsule radius.
	*/
	bool HasReachedPosition(ACharacter* Character, const FVector& TargetPosition) const;

	/** Callback when a movement is completed by the AI Controller. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|Move")
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult);

	/** Implementation of move request creation. */
	virtual FAIMoveRequest CreateMoveRequest(const USP_Target* Target);

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	bool End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_MoveToTask(const FObjectInitializer& ObjectInitializer);
};

/** Task info implementation for USP_MoveToTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Action|Task|Move")
class SPLANNER_API USP_MoveToTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_MoveToTask.
	friend USP_MoveToTask;

#if SP_DEBUG
	FNavPathSharedPtr DebugPath;
#endif

	FAIMoveRequest MoveRequest;
	AAIController* Controller = nullptr;

	/** The saved previous pawn speed. */
	float PrevPawnSpeed = -1.0f;

	/** Whether goal position must be re-compute each tick. */
	bool bIsDynamic = false;

	float DynamicTime = 0.0f;

	int MoveId = -1;

	/** Internal request execution state. */
	ESP_PlanExecutionState ExecutionState = ESP_PlanExecutionState::PES_Running;
};