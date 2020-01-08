#pragma once

#include <AIController.h>

#include <Tasks/Task.h>
#include "MoveToTask.generated.h"

/**
 *	MoveTo implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|MoveTo")
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|MoveTo")
	float AcceptanceRadius = 10.0f;

	/** Check if owner actor has reached its target. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Task|MoveTo")
	bool HasReachedPosition(const USP_PlannerComponent* Planner) const;

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
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|MoveTo")
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type ExecResult);

public:
	bool PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;
	FSP_PlannerFlags PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;

	ESP_PlanExecutionState Begin(USP_PlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState End(USP_PlannerComponent* Planner, uint8* UserData) override;

	bool Cancel(USP_PlannerComponent* Planner, uint8* UserData) override;
};