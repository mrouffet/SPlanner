#pragma once

#include <Engine/DataAsset.h>
#include "SP_Goal.generated.h"

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
	TArray<USP_Goal*> AllowedTransitions;

	/**
	*	Blocked transition list from this goal to a new one.
	*	Only used if AllowedTransitions is empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	TArray<USP_Goal*> BlockedTransitions;

public:
	/** Getter of AllowedTransitions. */
	const TArray<USP_Goal*>& GetAllowedTransitions() const;

	/** Getter of BlockedTransitions. */
	const TArray<USP_Goal*>& GetBlockedTransitions() const;

	/*
	*	Whether this goal can transit to NewGoal.
	*	return this not a sub-goal of NewGoal and transition allowed.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Goal")
	bool CanTransitTo(USP_Goal* NewGoal) const;
};