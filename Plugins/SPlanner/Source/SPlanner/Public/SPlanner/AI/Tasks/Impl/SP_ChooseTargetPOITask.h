// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetPOITask.generated.h"

class USP_POIComponent;

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
protected:
	/** Set the POI target into Planner. */
	void SetPOITarget(USP_AIPlannerComponent* Planner, USP_POIComponent* POI);

public:
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;
};