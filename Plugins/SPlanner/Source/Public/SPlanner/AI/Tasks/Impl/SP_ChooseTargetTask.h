#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_ChooseTargetTask.generated.h"

class USP_LookAtTask;

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_Task
{
	GENERATED_BODY()
	
	enum class ESP_InternalTickState : uint8
	{
		/** Active task, called each frame. */
		ITS_WaitForSuccess			UMETA(DisplayName = "WaitForSuccess"),

		/** Task end with success. */
		ITS_Tick					UMETA(DisplayName = "Succeed"),
	};

protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName TargetEntryName = "None";

	/**
	*	LookAt task to execute after target.
	*	Set nullptr for no look at.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	USP_LookAtTask* LookAtTask = nullptr;

	/** Can target again even if a previous target has been set in plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bAllowRetarget = false;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector LocalOffset;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector Dimensions = FVector(100.0f, 100.0f, 100.0f);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor DebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 2.0f;
#endif

	// The internal implementation of tick.
	virtual ESP_PlanExecutionState Tick_Internal(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData);

public:
	uint32 GetUserDataSize() const override;

	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;

	bool Begin(USP_AIPlannerComponent& Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData) override final;
	bool End(USP_AIPlannerComponent& Planner, uint8* UserData) override;

	bool Cancel(USP_AIPlannerComponent& Planner, uint8* UserData) override;
};