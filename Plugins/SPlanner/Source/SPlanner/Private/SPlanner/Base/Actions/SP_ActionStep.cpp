// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

#include <SPlanner/Base/Decorators/SP_Decorator.h>

bool USP_ActionStep::CheckAvailability(const USP_PlannerComponent* Planner) const
{
	// Check decorators all valid.
	for (int i = 0; i < AvailabilityDecorators.Num(); ++i)
	{
		SP_SCCHECK(AvailabilityDecorators[i], "AvailabilityDecorators[%d] nullptr!", i)

		if (!AvailabilityDecorators[i]->Available_Validate(Planner))
			return false;
	}

	return true;
}

bool USP_ActionStep::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(PlanGenInfos, false)

	// Check decorators all valid.
	for (int i = 0; i < PreConditionDecorators.Num(); ++i)
	{
		SP_SCCHECK(PreConditionDecorators[i], "PreConditionDecorators[%d] nullptr!", i)

		if (!PreConditionDecorators[i]->PreCondition_Validate(Planner, PlanGenInfos))
			return false;
	}

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