// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Action/SP_ActionImpl.h>

TArray<FSP_PlannerAction>& FSP_PlannerActionSet::GetActionsFromDepth(uint8 Depth)
{
	if (Depth == 0 && BeginActions.Num())
		return BeginActions;

	return Actions;
}
const TArray<FSP_PlannerAction>& FSP_PlannerActionSet::GetActionsFromDepth(uint8 Depth) const
{
	if (Depth == 0 && BeginActions.Num())
		return BeginActions;

	return Actions;
}

FSP_PlannerActionSet FSP_PlannerActionSet::Make(const USP_PlannerComponent* Planner, const USP_ActionSet* ActionSet, bool* bCanBeAchievedPtr)
{
	SP_SRCHECK_NULLPTR(Planner, FSP_PlannerActionSet())
	SP_SRCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

	FSP_PlannerActionSet Result;
	FSP_PlannerActionSortFunctor SortFunctor;


	// Begin Action shuffle.
	if (ActionSet->GetBeginActions().Num())
	{
		const TArray<USP_Action*>& BeginActions = ActionSet->GetBeginActions();

		for (int i = 0; i < BeginActions.Num(); ++i)
		{
			SP_SCCHECK(BeginActions[i], "%s.BeginActions[ %d ] is nullptr!", *ActionSet->GetName(), i)
			SP_SCCHECK(BeginActions[i]->Impl, "%s.BeginActions[ %d ].Impl is nullptr!", *ActionSet->GetName(), i)

			if (BeginActions[i]->IsAvailable(Planner))
				Result.BeginActions.Add(FSP_PlannerAction::Make(Planner, BeginActions[i]));
		}

		Result.BeginActions.Sort(SortFunctor);
	}


	// Core and End Action shuffle.
	{
		{
			const TArray<USP_Action*>& CoreActions = ActionSet->GetCoreActions();

			for (int i = 0; i < CoreActions.Num(); ++i)
			{
				SP_SCCHECK(CoreActions[i], "%s.CoreActions[ %d ] is nullptr!", *ActionSet->GetName(), i)
				SP_SCCHECK(CoreActions[i]->Impl, "%s.CoreActions[ %d ]->Impl is nullptr!", *ActionSet->GetName(), i)

				if (CoreActions[i]->IsAvailable(Planner))
					Result.Actions.Add(FSP_PlannerAction::Make(Planner, CoreActions[i]));
			}
		}

		{
			bool bCanBeAchieved = false;

			const TArray<USP_Action*>& EndActions = ActionSet->GetEndActions();

			for (int i = 0; i < EndActions.Num(); ++i)
			{
				SP_SCCHECK(EndActions[i], "%s.EndActions[ %d ] is nullptr!", *ActionSet->GetName(), i)
				SP_SCCHECK(EndActions[i]->Impl, "%s.EndActions[ %d ]->Impl is nullptr!", *ActionSet->GetName(), i)

				if (EndActions[i]->IsAvailable(Planner))
				{
					Result.Actions.Add(FSP_PlannerAction::Make(Planner, EndActions[i], true));
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
		const TArray<USP_Action*>& ForcedActions = ActionSet->GetForcedActions();

		// Compute forced action weights.
		TArray<FSP_PlannerAction> ForcedPlannerActions;

		for (int i = 0; i < ForcedActions.Num(); ++i)
		{
			SP_SCCHECK(ForcedActions[i], "%s.ForcedActions[ %d ] is nullptr!", *ActionSet->GetName(), i)
			SP_SCCHECK(ForcedActions[i]->Impl, "%s.ForcedActions[ %d ]->Impl is nullptr!", *ActionSet->GetName(), i)

			if (ForcedActions[i]->IsAvailable(Planner))
				ForcedPlannerActions.Add(FSP_PlannerAction::Make(Planner, ForcedActions[i], false, true));
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