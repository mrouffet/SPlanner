// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Animation/AnimCurveTypes.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>
#include "SP_AIFloatParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param")
class SPLANNER_API USP_AIFloatParam : public USP_FloatParam
{
	GENERATED_BODY()

protected:
	/**
	*	Getter of value using Curve with valid LODLevel, otherwise Default.
	*	Multiply with all Inputs.
	*/
	float Query_Implementation(const UObject* Outer) const override;

public:
	/**
	*	The weight of the action depending on the LOD level.
	*	Override DefaultWeight.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FRuntimeFloatCurve LODCurve;
};