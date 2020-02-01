// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_BoolParam.h>

#include <SPlanner/Base/Decorators/SP_Decorator.h>
#include "SP_BoolDecorator.generated.h"

/**
 *	SPlanner input bool decorator.
 */
UCLASS()
class SPLANNER_API USP_BoolDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** Handled param. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FSP_BoolParam Param;

	bool Validate_Internal_Implementation(const USP_PlannerComponent* Planner) override;
};