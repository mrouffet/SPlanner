// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_BoolDecorator.generated.h"

class USP_BoolParam;

/**
 *	SPlanner input bool decorator.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_BoolDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** Handled param. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_BoolParam* Param = nullptr;

	bool Validate_Internal_Implementation(const USP_PlannerComponent* Planner) override;
};