#pragma once

#include <SPlanner/Miscs/SP_Weight.h>

#include <SPlanner/Goals/SP_Goal.h>
#include "SP_WeightGoal.generated.h"

/**
 *	Goal implementation with a weight.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|Weight")
class SPLANNER_API USP_WeightGoal : public USP_Goal
{
	GENERATED_BODY()
	
public:
	/**
	*	The default weight of the goal.
	*	Increase Weight to increase chances of use for generation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|Weight")
	FSP_Weight Weight;
};