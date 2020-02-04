#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_Task
{
	GENERATED_BODY()
	
protected:
	/** Set auto rotation to target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bAutoLookAt = true;

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

public:
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};