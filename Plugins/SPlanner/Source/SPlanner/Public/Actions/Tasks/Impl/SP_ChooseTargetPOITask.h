#pragma once

#include <Actions/Tasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPOITask.generated.h"

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|Task|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
public:
	FSP_PlannerFlags PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};