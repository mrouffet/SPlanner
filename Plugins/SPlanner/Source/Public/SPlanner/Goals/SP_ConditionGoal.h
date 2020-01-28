#pragma once

#include <SPlanner/Goals/SP_Goal.h>
#include "SP_ConditionGoal.generated.h"

/**
 *	Goal implementation with a predicate to validate.
 *	Implement Predicate() in children BP.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|Condition")
class SPLANNER_API USP_ConditionGoal : public USP_Goal
{
	GENERATED_BODY()
	
public:
	/**
	*	The predicate to validate to use this for plan generation.
	*	Implement in children.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Goal|Condition")
	virtual bool Predicate(const USP_PlannerComponent* Planner) const;

	const USP_Goal* GetExecutionGoal(const USP_PlannerComponent* Planner) const override;
};