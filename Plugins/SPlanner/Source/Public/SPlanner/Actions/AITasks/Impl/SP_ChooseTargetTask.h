#pragma once

#include <SPlanner/Actions/AITasks/SP_AITask.h>
#include "SP_ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Action|AITask|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_AITask
{
	GENERATED_BODY()
	
protected:
	/** Set auto rotation to target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target")
	bool bAutoLookAt = true;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target")
	FVector LocalOffset;

	/** Local offset to apply to the center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AITask|Target")
	FVector Dimensions = FVector(100.0f, 100.0f, 100.0f);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|AITask|Target")
	FColor DebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|AITask|Target")
	float DebugDrawTime = 2.0f;
#endif

public:
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};