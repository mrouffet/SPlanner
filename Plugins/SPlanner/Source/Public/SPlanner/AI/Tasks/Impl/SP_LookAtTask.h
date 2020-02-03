#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_LookAtTask.generated.h"

/**
 *	Look at task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_LookAtTask : public USP_Task
{
	GENERATED_BODY()
	
	struct FSP_TaskInfos
	{
		FRotator Start;
		FRotator End;

		float Alpha = 0.0f;
	};

protected:
	/** Whether the rotation should be in one frame.*/
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bInstant = false;

	/** Rotation speed.*/
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "SPlanner|Task|Target", meta=(EditCondition="!bInstant"))
	float Speed = 4.0f;

public:
	uint32 GetUserDataSize() const override;

	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;


	bool Begin(USP_AIPlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
	bool End(USP_AIPlannerComponent* Planner, uint8* UserData) override;
};