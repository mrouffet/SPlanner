// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionSet.h>
#include "SP_TaskSet.generated.h"

/**
 *	Set of Planner Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_TaskSet : public USP_ActionSet
{
	GENERATED_BODY()

public:
	USP_TaskSet(const FObjectInitializer& ObjectInitializer);
};