#pragma once

#include <Tasks/Impl/ChooseTargetTask.h>
#include "ChooseTargetPOITask.generated.h"

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
public:
	int PostCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const override;

	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;
};