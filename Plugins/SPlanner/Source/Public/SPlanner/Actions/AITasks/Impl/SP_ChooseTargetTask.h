#pragma once

#include <SPlanner/Actions/AITasks/SP_AITask.h>
#include "SP_ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|PlanStep|AITask|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_AITask
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

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|ChooseTarget")
	FColor DebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|ChooseTarget")
	float DebugDrawTime = 2.0f;
#endif

public:
	bool PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};