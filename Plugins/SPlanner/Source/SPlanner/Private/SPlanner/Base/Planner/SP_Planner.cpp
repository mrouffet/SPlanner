// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Planner/SP_Planner.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_ActionStep.h>

bool SP_Planner::LinearConstruct(FSP_LinearConstructInfos Infos)
{
	SP_BENCHMARK_SCOPE(SP_LinearConstruct)

	return LinearConstruct_Internal(Infos, 0u, 0u);
}

bool SP_Planner::LinearConstruct_Internal(FSP_LinearConstructInfos& Infos, uint8 CurrDepth, uint64 PlannerFlags)
{
	if (CurrDepth > Infos.MaxDepth)
		return false;

	TArray<FSP_PlannerAction>& Actions = Infos.PlannerActions.GetActionsFromDepth(CurrDepth);

	for (int i = 0; i < Actions.Num(); ++i)
	{
		// Make copy because of future Remove.
		FSP_PlannerAction Action = Actions[i];

		// invalid pre-condition.
		if (!Action.Handle->Step->PreCondition(Infos.Planner, Infos.OutPlan, PlannerFlags))
			continue;

		// Add steps to current plan.
		Infos.OutPlan.Add(Action.Handle->Step);

		// Remove from source set.
		Actions.RemoveAt(i);

		uint64 ActionKey = UpdateSet(Infos, Action);

		// Check if action achieve plan or plan with this action can be achieved.
		if (Action.bAchievePlan || LinearConstruct_Internal(Infos, CurrDepth + 1u, Action.Handle->Step->PostCondition(Infos.Planner, PlannerFlags)))
		{
			// Check if all forced action has been correctly added.
			if (Infos.PlannerActions.bHasForcedActions && CurrDepth == 0u)
			{
				for (int j = 0; j < Infos.PlannerActions.Actions.Num(); ++j)
				{
					/**
					*	One forced action has been left in action set.
					*	Since ForcedAction.Weight > EndAction.Weight, a plan which succeed already tried every combination using forced actions.
					*/
					if (Infos.PlannerActions.Actions[j].bIsForced)
						return false;
				}
			}

			return true;
		}

		// Plan generation failed, remove this action from plan.
		Infos.OutPlan.RemoveAt(Infos.OutPlan.Num() - 1); // Remove last.
		Actions.Insert(Action, i);

		RollbackSet(Infos, Action, ActionKey);
	}

	return false;
}

uint64 SP_Planner::UpdateSet(FSP_LinearConstructInfos& Infos, const FSP_PlannerAction& Action)
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
		float MinWeight = Infos.PlannerActions.Actions[Infos.PlannerActions.Actions.Num() - 1].GeneratedWeight;
		FSP_PlannerAction& NewAction = Infos.PlannerActions.Actions.Add_GetRef(Action);
		NewAction.bIsForced = false; // Never copy forced action.

		// Set new min weight. PushBacked tasks always have weight < 0.0f.
		NewAction.GeneratedWeight = MinWeight < 0.0f ? MinWeight - 1.0f : -1.0f;

		ActionKey = Infos.PlannerActions.Actions.Num() - 1; // Store index.
		break;
	}
	case ESP_ActionGenerationType::AGT_RandomWeight:
	{
		FSP_PlannerAction NewAction = FSP_PlannerAction::Make(Action.Handle, Infos.LODLevel, Action.bAchievePlan);
		ActionKey = reinterpret_cast<uint64&>(NewAction.GeneratedWeight); // Store generated weight.

		Infos.PlannerActions.Actions.Add(NewAction);
		Infos.PlannerActions.Actions.Sort(FSP_PlannerActionSortFunctor());
		break;
	}
	default:
		SP_SLOG(Warning, "ActionGenerationType not supported yet!")
		break;
	}

	return ActionKey;
}
void SP_Planner::RollbackSet(FSP_LinearConstructInfos& Infos, const FSP_PlannerAction& Action, uint64 ActionKey)
{
	// Update Action list from generation type.
	// Always push in PlannerActions.Actions (this.Actions can be BeginActions).
	switch (Action.Handle->GenerationType)
	{
	case ESP_ActionGenerationType::AGT_OnceInPlan:
		break;
	case ESP_ActionGenerationType::AGT_PushBack:
		Infos.PlannerActions.Actions.RemoveAt(ActionKey); // Remove at index.
	case ESP_ActionGenerationType::AGT_RandomWeight:
		for (int j = 0; j < Infos.PlannerActions.Actions.Num(); ++j)
		{
			/**
			*	Compare action using GeneratedWeight.
			*	GeneratedWeight is never the exact same (random generation).
			*/
			if (FMath::IsNearlyEqual(reinterpret_cast<float&>(ActionKey), Infos.PlannerActions.Actions[j].GeneratedWeight))
			{
				Infos.PlannerActions.Actions.RemoveAt(j);
				break;
			}
		}

		break;
	default:
		SP_SLOG(Warning, "ActionGenerationType not supported yet!")
		break;
	}
}