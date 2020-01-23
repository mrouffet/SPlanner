#pragma once

#include <SPlanner/Actions/AITasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPOITask.generated.h"

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|AITask|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
public:
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};