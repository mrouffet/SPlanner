// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/Base/Actions/SP_ActionBase.h>
#include "SP_Action.generated.h"

/**
*	Action Base with Weight used to shuffle actions before plan generation.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_Action : public FSP_ActionBase
{
	GENERATED_BODY()

public:
	/**
	*	The default weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FSP_FloatParam Weight;

	FSP_Action();
};