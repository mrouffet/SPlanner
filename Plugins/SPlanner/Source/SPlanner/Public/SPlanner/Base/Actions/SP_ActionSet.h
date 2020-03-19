// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Actions/SP_Action.h>

#include <Engine/DataAsset.h>
#include "SP_ActionSet.generated.h"

struct FSP_PlannerActionSet;

class USP_PlannerComponent;

/**
 *	Set of Planner Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_ActionSet : public UDataAsset
{
	GENERATED_BODY()

protected:
	/**
	*	List of begin actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> BeginActions;

	/** 
	*	List of core actions.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> CoreActions;

	/** 
	*	List of forced actions.
	*	All of these will be added to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> ForcedActions;

	/**
	*	List of end actions.
	*	Only one of these will be selected.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_Action> EndActions;

public:
	/** Getter of BeginActions. */
	const TArray<FSP_Action>& GetBeginActions() const;

	/** Getter of CoreActions. */
	const TArray<FSP_Action>& GetCoreActions() const;

	/** Getter of ForcedCoreActions. */
	const TArray<FSP_Action>& GetForcedActions() const;

	/** Getter of EndActions. */
	const TArray<FSP_Action>& GetEndActions() const;

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*	Call FSP_PlannerActionSet::Make().
	*/
	FSP_PlannerActionSet Shuffle(const USP_PlannerComponent* Planner, float LODLevel = -1.0f, bool* bCanBeAchievedPtr = nullptr) const;
};