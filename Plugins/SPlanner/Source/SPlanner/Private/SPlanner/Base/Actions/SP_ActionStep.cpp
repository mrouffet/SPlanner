// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStep::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	if(PreviousRequieredSteps.Num())
	{
		if (bUseRequiredStepOrder)
		{
			// Not enough actions in generated plan.
			if (GeneratedPlan.Num() < PreviousRequieredSteps.Num())
				return false;

			for (int i = 0; i < PreviousRequieredSteps.Num(); ++i)
			{
				// PreviousRequieredSteps.Num() and GeneratedPlan.Num() can be different.
				if (PreviousRequieredSteps[PreviousRequieredSteps.Num() - 1 - i] != GeneratedPlan[GeneratedPlan.Num() - 1 - i])
					return false;
			}
		}
		else
		{
			for (int i = 0; i < PreviousRequieredSteps.Num(); ++i)
			{
				// Find each required in plan.
				if (GeneratedPlan.Find(PreviousRequieredSteps[i]) == INDEX_NONE)
					return false;
			}
		}
	}

	return true;
}
uint64 USP_ActionStep::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	return PlannerFlags;
}