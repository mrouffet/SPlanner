#pragma once

#include <Animation/AnimCurveTypes.h>

#include <SPlanner/Assets/SP_FloatAsset.h>

#include "SP_Weight.generated.h"

/**
*	Weight struct implementation.
*	Increase Weight to increase chances of adding the action to the plan.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Misc")
struct FSP_Weight
{
	GENERATED_BODY()

protected:
	/** The default weight. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float DefaultWeight = 1.0f;

	/**
	*	The weight of the action depending on the LOD level.
	*	Override DefaultWeight.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve WeightLODCurve;

	/**
	*	Additionnal input weights.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_FloatAsset*> InputWeights;

public:
	FSP_Weight() = default;

	/**
	*	Getter of Weight using WeightCurve with valid LODLevel, otherwise DefaultWeight.
	*	Multiply with all InputWeights.
	*/
	float Get(float LODLevel = -1.0f) const;
};