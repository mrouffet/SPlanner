// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionStepChain.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStepChain::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	/**
	*	PostCondition will modify PlanGenInfos.
	*	PostCondition must be added to check next PreConditions.
	*	Call ResetPostCondition to get back initial PlanGenInfos.
	*	Avoid PlanGenInfos copy.
	*/
	USP_PlanGenInfos* NonConstPlanGenInfos = const_cast<USP_PlanGenInfos*>(PlanGenInfos);

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		if (!Steps[i]->PreCondition(Planner, GeneratedPlan, PlanGenInfos) || !Steps[i]->PostCondition(Planner, NonConstPlanGenInfos))
		{
			// PostCondition failed: Reset all previously set.
		
			for (int j = i - 1; j >= 0; --j)
				Steps[j]->ResetPostCondition(Planner, NonConstPlanGenInfos);

			return false;
		}
	}


	// Reset NonConstPlanGenInfos to initial value.
	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		Steps[i]->ResetPostCondition(Planner, NonConstPlanGenInfos);
	}

	return true;
}
bool USP_ActionStepChain::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		// Precondition has already been checked.
		if (!Steps[i]->PostCondition(Planner, PlanGenInfos))
		{
			// Post Condition failed: Reset all previously set.

			for (int j = i - 1; j >= 0; --j)
				Steps[j]->ResetPostCondition(Planner, PlanGenInfos);

			return false;
		}
	}

	return true;
}

bool USP_ActionStepChain::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		Steps[i]->ResetPostCondition(Planner, PlanGenInfos);
	}

	return true;
}