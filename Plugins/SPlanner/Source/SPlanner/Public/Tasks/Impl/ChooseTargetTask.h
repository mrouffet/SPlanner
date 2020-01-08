#pragma once

#include <Tasks/Task.h>
#include "ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_Task
{
	GENERATED_BODY()
	
protected:
	/** Set auto rotation to target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|ChooseTarget")
	bool bAutoLookAt = true;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|ChooseTarget")
	FVector LocalOffset;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|ChooseTarget")
	FVector Dimensions = FVector(100.0f, 100.0f, 100.0f);

public:
	bool PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;
	FSP_PlannerFlags PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;
};