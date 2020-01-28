#pragma once

#include <Engine/DataAsset.h>
#include "SP_Goal.generated.h"

class USP_PlannerComponent;

/**
 *	Planner goal asset.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal")
class SPLANNER_API USP_Goal : public UDataAsset
{
	GENERATED_BODY()

protected:
	/**
	*	Allowed transition list from this goal to a new one.
	*	Let empty to allow transition with all other goals.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	TArray<const USP_Goal*> AllowedTransitions;

	/**
	*	Blocked transition list from this goal to a new one.
	*	Only used if AllowedTransitions is empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	TArray<const USP_Goal*> BlockedTransitions;

public:
	/** Getter of AllowedTransitions. */
	const TArray<const USP_Goal*>& GetAllowedTransitions() const;

	/** Getter of BlockedTransitions. */
	const TArray<const USP_Goal*>& GetBlockedTransitions() const;

	/**
	*	Get the goal to execute for plan generation.
	*	Return this. Overriden behavior in children (subgoal implementation).
	*/
	virtual const USP_Goal* GetExecutionGoal(const USP_PlannerComponent* Planner) const;

	/*
	*	Whether this goal can transit to NewGoal.
	*	return this not a sub-goal of NewGoal and transition allowed.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Goal")
	virtual bool CanTransitTo(const USP_Goal* NewGoal) const;
};