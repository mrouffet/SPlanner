// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/SP_MoveToTask.h>
#include "SP_MoveToPOITask.generated.h"

/**
 *	MoveTo POI only spec implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|MoveTo")
class SPLANNER_API USP_MoveToPOITask : public USP_MoveToTask
{
	GENERATED_BODY()

protected:
	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
};