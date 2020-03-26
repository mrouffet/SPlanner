// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionStepChain.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStepChain::IsAvailable(const USP_PlannerComponent* Planner) const
{
	if (!Super::IsAvailable(Planner))
		return false;

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Steps[i])

		if (!Steps[i]->IsAvailable(Planner))
			return false;
	}

	return true;
}

bool USP_ActionStepChain::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	/**
	*	PostCondition will modify Infos.
	*	PostCondition must be added to check next PreConditions.
	*	Call ResetPostCondition to get back initial Infos.
	*	Avoid Infos copy.
	*/
	USP_PlanGenInfos* NonConstPlanGenInfos = const_cast<USP_PlanGenInfos*>(Infos);

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Steps[i], false)

		if (!Steps[i]->PreCondition(Infos) || !Steps[i]->PostCondition(NonConstPlanGenInfos))
		{
			// PostCondition failed: Reset all previously set.
		
			for (int j = i - 1; j >= 0; --j)
				Steps[j]->ResetPostCondition(NonConstPlanGenInfos);

			return false;
		}
	}


	// Reset NonConstPlanGenInfos to initial value.
	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Steps[i], false)

		Steps[i]->ResetPostCondition(NonConstPlanGenInfos);
	}

	return true;
}
bool USP_ActionStepChain::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Steps[i], false)

		// Precondition has already been checked.
		if (!Steps[i]->PostCondition(Infos))
		{
			// Post Condition failed: Reset all previously set.

			for (int j = i - 1; j >= 0; --j)
				Steps[j]->ResetPostCondition(Infos);

			return false;
		}
	}

	return true;
}

bool USP_ActionStepChain::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Steps[i], false)

		Steps[i]->ResetPostCondition(Infos);
	}

	return true;
}