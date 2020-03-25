// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>
#include "SP_FloatCurveParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_FloatCurveParam : public USP_FloatParam
{
	GENERATED_BODY()

protected:
	float Query_Implementation(const UObject* Outer) const override;

public:
	/**
	*	The weight multiplier to apply over times added to plan.
	*	Require GenerationType == ESP_ActionGenerationType::AGT_RandomWeight.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FRuntimeFloatCurve Multiplier;

	USP_FloatCurveParam(const FObjectInitializer& ObjectInitializer);
};

UCLASS(BlueprintType, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_FloatCurveParamInfos : public UObject
{
	GENERATED_BODY()

public:
	// The outer object.
	const UObject* Outer = nullptr;

	// The value used to evaluate the curve.
	float CurveTime = -1.0f;
};