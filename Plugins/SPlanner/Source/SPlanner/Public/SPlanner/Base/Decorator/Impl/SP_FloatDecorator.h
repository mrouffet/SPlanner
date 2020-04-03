// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_FloatDecorator.generated.h"

class USP_FloatParam;

/**
 *	SPlanner input bool decorator.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_FloatDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** Handled param. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_FloatParam* Param = nullptr;

	/**
	*	The value to compare with Param.
	*	Validate: Param >= Threshold.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float Threshold = 0.0f;

	bool Validate_Internal_Implementation(const UObject* Object) override;
};