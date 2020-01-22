#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Actions/SP_ActionBase.h>
#include "SP_Action.generated.h"

/**
*	Action Base with Weight used to shuffle actions before plan generation.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_Action : public FSP_ActionBase
{
	GENERATED_BODY()

protected:
	/**
	*	The default weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float DefaultWeight = 1.0f;

	/**
	*	The weight of the action depending on the LOD level.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve WeightLODCurve;

public:
	FSP_Action() = default;

	/** Getter of Weight using WeightCurve with valid LODLevel, otherwise DefaultWeight. */
	float GetWeight(float LODLevel = -1.0f) const;
};