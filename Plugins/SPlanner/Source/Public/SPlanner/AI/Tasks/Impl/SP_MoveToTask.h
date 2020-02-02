#pragma once

#include <AIController.h>

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_MoveToTask.generated.h"

/**
 *	MoveTo implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|MoveTo")
class SPLANNER_API USP_MoveToTask : public USP_Task
{
	GENERATED_BODY()
	
	struct FSP_TaskInfos
	{
#if SP_DEBUG
		FNavPathSharedPtr DebugPath;
#endif

		AAIController* Controller = nullptr;

		/** Internal request execution state. */
		ESP_PlanExecutionState ExecutionState = ESP_PlanExecutionState::PES_Running;
	};

	/**
	*	Map of RequestID, task infos.
	*	Used to get back task infos on move completed.
	*/
	TMap<int, FSP_TaskInfos*> RequestIDToTaskInfos;

protected:
	/** The radius to accept the move to as completed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|MoveTo")
	float AcceptanceRadius = 10.0f;

	/** Check if owner actor has reached its target. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Action|Task|MoveTo")
	bool HasReachedPosition(const USP_AIPlannerComponent* Planner) const;

	/**
	*	Check if owner actor has reached its target.
	*	Internal implementation.
	*/
	bool HasReachedPosition(AActor* Actor, const FVector& TargetPosition) const;

	/**
	*	Check if owner character has reached its target.
	*	Internal implementation with capsule radius.
	*/
	bool HasReachedPosition(ACharacter* Character, const FVector& TargetPosition) const;

	uint32 GetUserDataSize() const override;

	/** Callback when a movement is completed by the AI Controller. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Action|Task|MoveTo")
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult);

	/** Implementation of move request creation. */
	virtual FAIMoveRequest CreateMoveRequest(USP_AIPlannerComponent* Planner);

public:
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	bool Begin(USP_AIPlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
	bool End(USP_AIPlannerComponent* Planner, uint8* UserData) override;

	bool Cancel(USP_AIPlannerComponent* Planner, uint8* UserData) override;
};