// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetActorTask.h>
#include "SP_ChooseTargetPOITask.generated.h"

class USP_POIComponent;

/**
 *	Choose target implementation with POI spec.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetPOITask : public USP_ChooseTargetActorTask
{
	GENERATED_BODY()
	
protected:
	/** Set the POI target into Planner. */
	void SetPOITarget(USP_AIPlannerComponent* Planner, USP_POIComponent* POI);

	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
};