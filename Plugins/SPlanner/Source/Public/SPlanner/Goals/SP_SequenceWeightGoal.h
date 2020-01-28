#pragma once

#include <SPlanner/Goals/SP_Goal.h>
#include "SP_SequenceWeightGoal.generated.h"

class USP_WeightGoal;

/**
 *	Sequence of sub-SP_WeightGoal.
 *	Use weights and random to chose goal for plan generation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|Sequence|Weight")
class SPLANNER_API USP_SequenceWeightGoal : public USP_Goal
{
	GENERATED_BODY()
	
public:
	/**
	*	List of sub goals.
	*	Use weights and random.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|Sequence|Weight")
	TArray<const USP_WeightGoal*> SubGoals;

public:
	const USP_Goal* GetExecutionGoal(const USP_PlannerComponent* Planner) const override;
};