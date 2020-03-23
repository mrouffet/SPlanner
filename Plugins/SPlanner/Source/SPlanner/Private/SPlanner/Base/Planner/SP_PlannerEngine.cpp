// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Planner/SP_PlannerEngine.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Action/SP_ActionStep.h>
#include <SPlanner/Base/Action/SP_PlannerAction.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

bool SP_PlannerEngine::LinearConstruct(USP_PlanGenInfos* Infos)
{
	SP_BENCHMARK_SCOPE(SP_LinearConstruct)

	return LinearConstruct_Internal(Infos, 0u);
}

bool SP_PlannerEngine::LinearConstruct_Internal(USP_PlanGenInfos* Infos, uint8 CurrDepth)
{
	if (CurrDepth > Infos->MaxDepth)
		return false;

	TArray<FSP_PlannerAction>& Actions = Infos->PlannerActions.GetActionsFromDepth(CurrDepth);

	for (int i = 0; i < Actions.Num(); ++i)
	{
		// Make copy because of future Remove.
		FSP_PlannerAction Action = Actions[i];

		// invalid pre-condition or post condition.
		if (!Action.Handle->Step->PreCondition(Infos) || !Action.Handle->Step->PostCondition(Infos))
			continue;

		// Add steps to current plan.
		Infos->OutPlan.Add(Action.Handle->Step);

		// Remove from source set.
		Actions.RemoveAt(i);

		uint64 ActionKey = UpdateSet(Infos, Action);

		// Check if action achieve plan
		if (Action.bAchievePlan)
		{
			// Check if all forced action has been correctly added.
			if (Infos->PlannerActions.bHasForcedActions)
			{
				bool bAllForcedActionAdded = true;

				for (int j = 0; j < Infos->PlannerActions.Actions.Num(); ++j)
				{
					/**
					*	One forced action has been left in action set.
					*	Since ForcedAction.Weight > EndAction.Weight, a plan which succeed already tried every combination using forced actions.
					*/
					if (Infos->PlannerActions.Actions[j].bIsForced)
					{
						bAllForcedActionAdded = false;
						break;
					}
				}

				// No forced action left: success.
				if (bAllForcedActionAdded)
					return true;
			}
			else // No forced action to check: success.
				return true;
		}
		// Check if plan with this action can be achieved.
		else if (LinearConstruct_Internal(Infos, CurrDepth + 1u))
			return true;

		// Plan generation failed, remove this action from plan.
		Infos->OutPlan.RemoveAt(Infos->OutPlan.Num() - 1); // Remove last.
		Actions.Insert(Action, i);

		RollbackSet(Infos, Action, ActionKey);
	}

	return false;
}

uint64 SP_PlannerEngine::UpdateSet(USP_PlanGenInfos* Infos, const FSP_PlannerAction& Action)
{
	uint64 ActionKey = 0u;

	// Update Action list from generation type.
	// Always push in PlannerActions.Actions (this.Actions can be BeginActions).
	switch (Action.Handle->GenerationType)
	{
	case ESP_ActionGenerationType::AGT_OnceInPlan:
		break;
	case ESP_ActionGenerationType::AGT_PushBack:
	{
		// Get Last action for min weight.
		float MinWeight = Infos->PlannerActions.Actions[Infos->PlannerActions.Actions.Num() - 1].GeneratedWeight;
		FSP_PlannerAction& NewAction = Infos->PlannerActions.Actions.Add_GetRef(Action);
		NewAction.bIsForced = false; // Never copy forced action.

		// Set new min weight. PushBacked tasks always have weight < 0.0f.
		NewAction.GeneratedWeight = MinWeight < 0.0f ? MinWeight - 1.0f : -1.0f;

		ActionKey = Infos->PlannerActions.Actions.Num() - 1; // Store index.
		break;
	}
	case ESP_ActionGenerationType::AGT_RandomWeight:
	{
		FSP_PlannerAction NewAction = FSP_PlannerAction::Make(Infos->Planner, Action);
		ActionKey = reinterpret_cast<uint64&>(NewAction.GeneratedWeight); // Store generated weight.

		Infos->PlannerActions.Actions.Add(NewAction);
		Infos->PlannerActions.Actions.Sort(FSP_PlannerActionSortFunctor());
		break;
	}
	default:
		SP_SLOG(Warning, "ActionGenerationType not supported yet!")
		break;
	}

	return ActionKey;
}
void SP_PlannerEngine::RollbackSet(USP_PlanGenInfos* Infos, const FSP_PlannerAction& Action, uint64 ActionKey)
{
	// Update Action list from generation type.
	// Always push in PlannerActions.Actions (this.Actions can be BeginActions).
	switch (Action.Handle->GenerationType)
	{
	case ESP_ActionGenerationType::AGT_OnceInPlan:
		break;
	case ESP_ActionGenerationType::AGT_PushBack:
		Infos->PlannerActions.Actions.RemoveAt(ActionKey); // Remove at index.
	case ESP_ActionGenerationType::AGT_RandomWeight:
		for (int j = 0; j < Infos->PlannerActions.Actions.Num(); ++j)
		{
			/**
			*	Compare action using GeneratedWeight.
			*	GeneratedWeight is never the exact same (random generation).
			*/
			if (FMath::IsNearlyEqual(reinterpret_cast<float&>(ActionKey), Infos->PlannerActions.Actions[j].GeneratedWeight))
			{
				Infos->PlannerActions.Actions.RemoveAt(j);
				break;
			}
		}

		break;
	default:
		SP_SLOG(Warning, "ActionGenerationType not supported yet!")
		break;
	}
}