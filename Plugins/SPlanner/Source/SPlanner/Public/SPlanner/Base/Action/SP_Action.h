// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_ActionBase.h>
#include "SP_Action.generated.h"

class USP_FloatParam;

/**
*	Action Base with Weight used to shuffle actions before plan generation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_Action : public USP_ActionBase
{
	GENERATED_BODY()

public:
	/**
	*	The default weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_FloatParam* Weight;
};