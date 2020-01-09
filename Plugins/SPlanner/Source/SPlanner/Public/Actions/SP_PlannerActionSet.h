#pragma once

#include <Actions/SP_PlannerAction.h>

#include "SP_PlannerActionSet.generated.h"

// Sort using weight.
struct ActionSortFunctor
{
	// Decreasing weight sort.
	bool operator()(const FSP_Action& lhs, const FSP_Action& rhs) const
	{
		return lhs.Weight > rhs.Weight;
	}
};

/**
*	Shuffled action using random and weights.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_PlannerActionSet
{
	GENERATED_BODY()

	/**
	*	Begin actions.
	*	Only one of these will be selected by the planner.
	*/
	TArray<FSP_PlannerAction> BeginActions;

	/**
	*	Forced actions.
	*	All of these will be added to the plan before trying to end it.
	*/
	TArray<FSP_PlannerAction> ForcedActions;

	/**
	*	All core and end action.
	*	The planner will end the generation while a end action will be added.
	*/
	TArray<FSP_PlannerAction> Actions;

	FSP_PlannerActionSet() = default;

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*/
	template <typename PredicateClass>
	FSP_PlannerActionSet(const TArray<FSP_Action>& InBeginActions, const TArray<FSP_Action>& InCoreActions,
		const TArray<FSP_Action>& InForcedCoreActions, const TArray<FSP_Action>& InEndActions, const PredicateClass& IsAvailablePredicate)
	{
		ActionSortFunctor SortFunctor;

		// Begin Action shuffle.
		for (int i = 0; i < InBeginActions.Num(); ++i)
		{
			if(IsAvailablePredicate(InBeginActions[i]))
				BeginActions.Add(FSP_PlannerAction(InBeginActions[i].Step, InBeginActions[i].Weight * FMath::FRand()));
		}

		BeginActions.Sort(SortFunctor);


		// Forced action shuffle
		for (int i = 0; i < InForcedCoreActions.Num(); ++i)
		{
			if (IsAvailablePredicate(InForcedCoreActions[i]))
				ForcedActions.Add(FSP_PlannerAction(InForcedCoreActions[i].Step, InForcedCoreActions[i].Weight * FMath::FRand()));
		}

		ForcedActions.Sort(SortFunctor);


		// Core and end Action shuffle.
		for (int i = 0; i < InCoreActions.Num(); ++i)
		{
			if (IsAvailablePredicate(InCoreActions[i]))
				Actions.Add(FSP_PlannerAction(InCoreActions[i].Step, InCoreActions[i].Weight * FMath::FRand()));
		}

		for (int i = 0; i < InEndActions.Num(); ++i)
		{
			if (IsAvailablePredicate(InEndActions[i]))
				Actions.Add(FSP_PlannerAction(InEndActions[i].Step, InEndActions[i].Weight * FMath::FRand(), true));
		}

		Actions.Sort(SortFunctor);
	}

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*	Use default predicate (always true).
	*/
	FSP_PlannerActionSet(const TArray<FSP_Action>& InBeginActions, const TArray<FSP_Action>& InCoreActions,
		const TArray<FSP_Action>& InForcedCoreActions, const TArray<FSP_Action>& InEndActions);
};