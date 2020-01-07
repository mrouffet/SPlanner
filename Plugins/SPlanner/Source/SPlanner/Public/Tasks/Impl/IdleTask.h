#pragma once

#include <Tasks/Task.h>
#include "IdleTask.generated.h"

/**
 *	Idle task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_IdleTask : public USP_Task
{
	GENERATED_BODY()
	
	struct FSP_TaskInfos
	{
		float WaitTime = -1.0f;
		float CurrTime = 0.0f;
	};

protected:
	/** The minimum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Idle")
	float MinTime = 0.5f;

	/** The maximum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Idle")
	float MaxTime = 3.0f;

public:
	uint32 GetUserDataSize() const override;

	ESP_PlanExecutionState Begin(USP_PlannerComponent* Planner, uint8* UserData) override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;

	ESP_PlanExecutionState End(USP_PlannerComponent* Planner, uint8* UserData) override;
};