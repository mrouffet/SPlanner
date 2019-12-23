#pragma once

#include <Tasks/Impl/ChooseTargetTask.h>
#include "ChooseTargetPositionTask.generated.h"

/**
 *	Choose target implementation with position spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetPositionTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
public:
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;
};