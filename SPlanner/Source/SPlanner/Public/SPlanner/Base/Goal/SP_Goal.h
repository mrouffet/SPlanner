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
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Goal")
	TArray<USP_PlannerComponent*> Planners;

	/**
	*	Allowed input transition list (from old goal to this).
	*	Let empty to allow transition with all other goals.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|In")
	TArray<USP_Goal*> AllowedInTransitions;

	/**
	*	Blocked input transition list (from old goal to this).
	*	Only used if AllowedTransitions is empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|In")
	TArray<USP_Goal*> BlockedInTransitions;

	/**
	*	Allowed output transition list (from this goal to a new one).
	*	Let empty to allow transition with all other goals.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|Out")
	TArray<USP_Goal*> AllowedOutTransitions;

	/**
	*	Blocked output transition list (from this goal to a new one).
	*	Only used if AllowedTransitions is empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|Out")
	TArray<USP_Goal*> BlockedOutTransitions;

	/** Whether Planner can start this goal. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SPlanner|Goal|In")
	bool CanStart(USP_PlannerComponent* Planner) const;

	/** Whether Planner can leave this goal. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SPlanner|Goal|Out")
	bool CanLeave(USP_PlannerComponent* Planner) const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Getter of bResetBlackboard. */
	bool GetResetBlackboard() const;

	/** Getter of Planners. */
	const TArray<USP_PlannerComponent*>& GetPlanners() const;

	/** Callback method called when Planner starts this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Goal")
	void OnStart(USP_PlannerComponent* Planner);

	/** Callback method called when Planner ends this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Goal")
	void OnEnd(USP_PlannerComponent* Planner);

	/** Reset this formation set. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Goal")
	void Reset();

	/**
	*	Whether Planner can transit from OldGoal to NewGoal.
	*	Use bForce to avoid In and Out transition check.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Goal")
	static bool CanTransitTo(USP_PlannerComponent* Planner, USP_Goal* OldGoal, USP_Goal* NewGoal, bool bForce = false);
};