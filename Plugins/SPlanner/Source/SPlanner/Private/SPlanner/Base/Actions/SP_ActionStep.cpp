// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

bool USP_ActionStep::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(PlanGenInfos, false)

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
bool USP_ActionStep::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(PlanGenInfos, false)

	return true;
}

bool USP_ActionStep::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(PlanGenInfos, false)

	return true;
}