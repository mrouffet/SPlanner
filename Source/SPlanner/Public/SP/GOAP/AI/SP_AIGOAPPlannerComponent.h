// Copyright 2023 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SP/Base/AI/SP_BrainComponent.h>
#include "SP_GOAPPlannerComponent.generated.h"

/**
*	SPlanner AI GOAP planner component implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI|GOAP")
class SPLANNER_API USP_AIGOAPPlannerComponent : public USP_BrainComponent
{
	GENERATED_BODY()
};
