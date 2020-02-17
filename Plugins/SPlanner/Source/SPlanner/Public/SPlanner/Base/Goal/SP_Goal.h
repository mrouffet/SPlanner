// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

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
	/** Whether blackboard must be reset when planner start this goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	bool bResetBlackboard = true;

	/** All planner registered (currently using this goal). */
	TArray<USP_PlannerComponent*> Planners;

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
	/** Getter of bResetBlackboard. */
	bool GetResetBlackboard() const;

	/** Getter of Planners. */
	const TArray<USP_PlannerComponent*>& GetPlanners() const;

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

	/** Callback method called when Planner starts this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Goal")
	void OnStart(USP_PlannerComponent* Planner);

	/** Callback method called when Planner ends this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Goal")
	void OnEnd(USP_PlannerComponent* Planner);

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Goal")
	void Reset();
};