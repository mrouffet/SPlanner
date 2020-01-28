#pragma once

#include <SPlanner/Goals/SP_Goal.h>
#include "SP_SequenceConditionGoal.generated.h"

class USP_ConditionGoal;

/**
 *	Sequence of sub-SP_ConditionGoal.
 *	Use first goal which valid Predicate().
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|Sequence|Condition")
class SPLANNER_API USP_SequenceConditionGoal : public USP_Goal
{
	GENERATED_BODY()

protected:
	/**
	*	List of sub goals.
	*	Use first goal which valid Predicate().
	*	Keep order of iteration.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|Sequence|Condition")
	TArray<const USP_ConditionGoal*> SubGoals;

public:
	const USP_Goal* GetExecutionGoal(const USP_PlannerComponent* Planner) const override;
};