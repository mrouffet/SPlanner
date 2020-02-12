// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Animation/AnimCurveTypes.h>

#include <SPlanner/Miscs/Assets/SP_FloatAsset.h>

#include "SP_FloatParam.generated.h"

/**
*	float parameter struct implementation.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Misc|Param")
struct SPLANNER_API FSP_FloatParam
{
	GENERATED_BODY()

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Default = 1.0f;

	/**
	*	The weight of the action depending on the LOD level.
	*	Override DefaultWeight.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve LODCurve;

	/**
	*	Additionnal input values.
	*	The average input value will be computed and then multiplied to the current value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_FloatAsset*> Inputs;

	FSP_FloatParam() = default;

	/**
	*	Getter of value using Curve with valid LODLevel, otherwise Default.
	*	Multiply with all InputWeights.
	*/
	float Get(float LODLevel = -1.0f) const;
};