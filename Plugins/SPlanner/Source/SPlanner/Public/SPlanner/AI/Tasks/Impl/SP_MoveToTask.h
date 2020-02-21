// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <AIController.h>

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_MoveToTask.generated.h"

class USP_Target;

/**
 *	MoveTo implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|MoveTo")
class SPLANNER_API USP_MoveToTask : public USP_Task
{
	GENERATED_BODY()
	
protected:
	struct FSP_MoveToTaskInfos : FSP_TaskInfos
	{
#if SP_DEBUG
		FNavPathSharedPtr MT_DebugPath;
#endif

		FAIMoveRequest MT_MoveRequest;
		AAIController* MT_Controller = nullptr;

		/** Whether goal position must be re-compute each tick. */
		bool MT_bIsDynamic = false;

		float MT_DynamicTime = 0.0f;

		/** The saved previous pawn speed. */
		float MT_PrevPawnSpeed = -1.0f;

		/** Internal request execution state. */
		ESP_PlanExecutionState MT_ExecutionState = ESP_PlanExecutionState::PES_Running;
	};

	/**
	*	Map of RequestID, task infos.
	*	Used to get back task infos on move completed.
	*/
	TMap<int, FSP_MoveToTaskInfos*> RequestIDToTaskInfos;

	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	FName TargetEntryName = "MainTarget";

	
	/** Whether the target should be visible (perform raycast). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bTargetVisible = false;

	/** Should use pathfinding. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bUsePathfinding = true;

	/** Whether can strafe during move to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bCanStrafe = false;


	/** The radius to accept the move to as completed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	float AcceptanceRadius = 10.0f;

	/**
	*	The minimum distance from MainTarget.
	*	Set < 0.0f to disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	float MinDistance = -1.0f;

	/**
	*	The maximum distance from MainTarget.
	*	Set < 0.0f to disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	float MaxDistance = -1.0f;

	/**
	*	The pawn speed to set.
	*	Set < 0.0f to disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	float PawnSpeed = -1.0f;

	/** Can move to again even if a previous move to has been set in plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bAllowMultipleMoveTo = true;

	/** Should re-compute MoveTo on tick (only used when Target is not an actor). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bIsDynamic = true;

	/**
	*	The frequency of update while bIsDynamic it true.
	*	Use < 0.0f for every tick.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo", meta=(EditCondition="bIsDynamic"))
	float DynamicUpdateFrequency = -1.0f;

	/**
	*	Whether precondition should fail if Pawn is already at goal.
	*	Mostly use true, should use false when using Formation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	bool bPreconditionFailWhileAlreadyAtGoal = true;

	/** Check if owner actor has reached its target. */
	bool HasReachedPosition(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const;

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

	/** Whether the target is visible from Planner's pawn. */
	bool IsTargetVisible(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const;
	
	/** Child implementation to get pawn speed. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|MoveTo")
	float GetPawnSpeed(APawn* Pawn);

	/** Child implementation to set pawn speed during MoveTo. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|MoveTo")
	void SetPawnSpeed(APawn* Pawn, float NewPawnSpeed);

	uint32 GetUserDataSize() const override;

	/** Callback when a movement is completed by the AI Controller. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|MoveTo")
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult);

	/** Implementation of move request creation. */
	virtual FAIMoveRequest CreateMoveRequest(const USP_Target* Target);

	void ConstructUserData(uint8* UserData) override;
	void DestructUserData(uint8* UserData) override;

public:
	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;

	bool Begin(USP_AIPlannerComponent& Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData) override;
	bool End(USP_AIPlannerComponent& Planner, uint8* UserData) override;

	bool Cancel(USP_AIPlannerComponent& Planner, uint8* UserData) override;
};