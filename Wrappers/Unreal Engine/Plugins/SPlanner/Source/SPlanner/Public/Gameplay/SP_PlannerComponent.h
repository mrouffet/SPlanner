// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Abstract/SP_ActorComponent.h>

#include <GOAP/Base/Goal/SP_Goal.h>

#include "SP_PlannerComponent.generated.h"

UCLASS(DisplayName = "Planner", ClassGroup = "SPlanner")
class SPLANNER_API USP_PlannerComponent : public USP_ActorComponent
{
	GENERATED_BODY()

protected:
	/**
	*	The current targeted goal used for plan generation.
	*	Editor set default value.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* Goal = nullptr;
};
