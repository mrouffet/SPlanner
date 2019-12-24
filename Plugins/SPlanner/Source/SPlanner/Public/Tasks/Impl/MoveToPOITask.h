#pragma once

#include <Tasks/Impl/MoveToTask.h>
#include "MoveToPOITask.generated.h"

/**
 *	MoveTo POI only spec implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|MoveTo")
class SPLANNER_API UMoveToPOITask : public USP_MoveToTask
{
	GENERATED_BODY()

public:
	bool PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const override;
};