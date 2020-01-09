#pragma once

#include <Actions/Tasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPositionTask.generated.h"

/**
 *	Choose target implementation with position spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|PlanStep|Task|Target")
class SPLANNER_API USP_ChooseTargetPositionTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
public:
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
};