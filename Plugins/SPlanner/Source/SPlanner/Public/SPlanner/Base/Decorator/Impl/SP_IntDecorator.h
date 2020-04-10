// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_IntDecorator.generated.h"

class USP_IntParam;

/**
 *	SPlanner input bool decorator.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_IntDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** Handled param. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_IntParam* Param = nullptr;

	/**
	*	The value to compare with Param.
	*	Validate: Param >= Threshold.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	int Threshold = 0;

	bool Validate_Internal_Implementation(const UObject* Object) override;
};