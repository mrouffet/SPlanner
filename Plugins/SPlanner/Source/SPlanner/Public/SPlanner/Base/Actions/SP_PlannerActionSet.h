// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

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
	*	All possible actions (include core, forced and end).
	*	The planner will end the generation while a end action will be added.
	*/
	TArray<FSP_PlannerAction> Actions;

	/**
	*	Whether this action set contain forced actions.
	*	While true, perform additionnal check to ensure every forced action has been added to the plan.
	*/
	bool bHasForcedActions = false;

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
	static FSP_PlannerActionSet Make(const USP_ActionSet* ActionSet, float LODLevel, const PredicateClass& IsAvailablePredicate, bool* bCanBeAchievedPtr = nullptr)
	{
		SP_SRCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

		FSP_PlannerActionSet Result;
		FSP_PlannerActionSortFunctor SortFunctor;


		// Begin Action shuffle.
		if (ActionSet->GetBeginActions().Num())
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


		// Core and End Action shuffle.
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
				bool bCanBeAchieved = false;

				const TArray<FSP_Action>& EndActions = ActionSet->GetEndActions();

				for (int i = 0; i < EndActions.Num(); ++i)
				{
					SP_SCCHECK(EndActions[i].Step, "%s.EndActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)

					if (IsAvailablePredicate(EndActions[i]))
					{
						Result.Actions.Add(FSP_PlannerAction::Make(&EndActions[i], LODLevel, true));
						bCanBeAchieved = true;
					}
				}

				// Action set can't achieve goal (due to IsAvailablePredicate).
				if (!bCanBeAchieved)
				{
					if (bCanBeAchievedPtr)
						*bCanBeAchievedPtr = false;

					return Result;
				}
				else if (bCanBeAchievedPtr)
					*bCanBeAchievedPtr = true;
			}

			Result.Actions.Sort(SortFunctor);
		}


		// Forced Action shuffle.
		if (ActionSet->GetForcedActions().Num())
		{
			const TArray<FSP_Action>& ForcedActions = ActionSet->GetForcedActions();

			// Compute forced action weights.
			TArray<FSP_PlannerAction> ForcedPlannerActions;

			for (int i = 0; i < ForcedActions.Num(); ++i)
			{
				SP_SCCHECK(ForcedActions[i].Step, "%s.ForcedActions[ %d ].Step is nullptr!", *ActionSet->GetName(), i)

				if (IsAvailablePredicate(ForcedActions[i]))
					ForcedPlannerActions.Add(FSP_PlannerAction::Make(&ForcedActions[i], LODLevel, false, true));
			}

			ForcedPlannerActions.Sort(SortFunctor);

			if (ForcedPlannerActions.Num())
			{
				Result.bHasForcedActions = true;

				// Search for first end action of plan.
				int FirstEndActionIndex = -1;

				for (int i = 0; i < Result.Actions.Num(); ++i)
				{
					if (Result.Actions[i].bAchievePlan)
					{
						FirstEndActionIndex = i;
						break;
					}
				}

				SP_SRCHECK(FirstEndActionIndex >= 0, Result, "No end action found! This should never happen.")

				// Add forced action to action set.
				for (int i = 0; i < ForcedPlannerActions.Num(); ++i)
				{
					// Forced action must be tried to be added before any end attempt.
					if (ForcedPlannerActions[i].GeneratedWeight < Result.Actions[FirstEndActionIndex].GeneratedWeight)
					{
						// Compute new GeneratedWeight to set forced action's weight before any end action.
						if (FirstEndActionIndex > 0)
						{
							// Average weight.
							ForcedPlannerActions[i].GeneratedWeight =
								(Result.Actions[FirstEndActionIndex - 1].GeneratedWeight + Result.Actions[FirstEndActionIndex].GeneratedWeight) / 2.0f;
						}
						else
						{
							// Insert behind first end action (ensure good weight computation for any next loop).
							ForcedPlannerActions[i].GeneratedWeight = Result.Actions[FirstEndActionIndex].GeneratedWeight + 1.0f; // Add + 1.0f bias.
							Result.Actions.Insert(ForcedPlannerActions[i], FirstEndActionIndex);
							++FirstEndActionIndex;

							continue;
						}
					}

					Result.Actions.Add(ForcedPlannerActions[i]);
				}

				// Sort again to update Forced action order.
				Result.Actions.Sort(SortFunctor);
			}
		}

		return Result;
	}

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*	Use default predicate (always true).
	*/
	static FSP_PlannerActionSet Make(const USP_ActionSet* ActionSet, float LODLevel, bool* bCanBeAchievedPtr = nullptr);
};