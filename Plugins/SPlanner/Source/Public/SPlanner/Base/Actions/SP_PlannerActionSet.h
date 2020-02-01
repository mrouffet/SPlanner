#pragma once

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_ActionSet.h>
#include <SPlanner/Base/Actions/SP_PlannerAction.h>

#include "SP_PlannerActionSet.generated.h"

// Sort using weight.
struct SPLANNER_API FSP_PlannerActionSortFunctor
{
	// Decreasing weight sort.
	bool operator()(const FSP_PlannerAction& lhs, const FSP_PlannerAction& rhs) const
	{
		return lhs.GeneratedWeight > rhs.GeneratedWeight;
	}
};

/**
*	Shuffled action using random and weights.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_PlannerActionSet
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

	/** Getter of Action array depending on Depth. */
	TArray<FSP_PlannerAction>& GetActionsFromDepth(uint8 Depth);
	
	/** const Getter of Action array depending on Depth. */
	const TArray<FSP_PlannerAction>& GetActionsFromDepth(uint8 Depth) const;

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*/
	template <typename PredicateClass>
	static FSP_PlannerActionSet Make(const USP_ActionSet* ActionSet, float LODLevel, const PredicateClass& IsAvailablePredicate)
	{
		SP_SRCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

		FSP_PlannerActionSet Result;
		FSP_PlannerActionSortFunctor SortFunctor;


		// Begin Action shuffle.
		{
			const TArray<FSP_Action>& BeginActions = ActionSet->GetBeginActions();

			for (int i = 0; i < BeginActions.Num(); ++i)
			{
				SP_SCCHECK(BeginActions[i].Step, "%s.BeginActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)

				if (IsAvailablePredicate(BeginActions[i]))
					Result.BeginActions.Add(FSP_PlannerAction::Make(&BeginActions[i], LODLevel));
			}

			Result.BeginActions.Sort(SortFunctor);
		}


		// Forced action shuffle
		{
			const TArray<FSP_Action>& ForcedActions = ActionSet->GetForcedActions();

			for (int i = 0; i < ForcedActions.Num(); ++i)
			{
				SP_SCCHECK(ForcedActions[i].Step, "%s.ForcedActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)

				if (IsAvailablePredicate(ForcedActions[i]))
					Result.ForcedActions.Add(FSP_PlannerAction::Make(&ForcedActions[i], LODLevel));
			}

			Result.ForcedActions.Sort(SortFunctor);
		}

		// Core and end Action shuffle.
		{
			{
				const TArray<FSP_Action>& CoreActions = ActionSet->GetCoreActions();

				for (int i = 0; i < CoreActions.Num(); ++i)
				{
					SP_SCCHECK(CoreActions[i].Step, "%s.CoreActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)
				
					if (IsAvailablePredicate(CoreActions[i]))
						Result.Actions.Add(FSP_PlannerAction::Make(&CoreActions[i], LODLevel));
				}
			}

			{
				const TArray<FSP_Action>& EndActions = ActionSet->GetEndActions();

				for (int i = 0; i < EndActions.Num(); ++i)
				{
					SP_SCCHECK(EndActions[i].Step, "%s.EndActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)

					if (IsAvailablePredicate(EndActions[i]))
						Result.Actions.Add(FSP_PlannerAction::Make(&EndActions[i], LODLevel, true));
				}
			}

			Result.Actions.Sort(SortFunctor);
		}

		return Result;
	}

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*	Use default predicate (always true).
	*/
	static FSP_PlannerActionSet Make(const USP_ActionSet* ActionSet, float LODLevel);
};