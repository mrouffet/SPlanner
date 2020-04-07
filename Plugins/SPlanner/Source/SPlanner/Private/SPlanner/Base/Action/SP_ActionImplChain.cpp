// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionImplChain.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionImplChain::IsAvailable(const USP_PlannerComponent* Planner) const
{
	if (!Super::IsAvailable(Planner))
		return false;

	for (int i = 0; i < Impls.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Impls[i])

		if (!Impls[i]->IsAvailable(Planner))
			return false;
	}

	return true;
}

bool USP_ActionImplChain::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	SP_RCHECK(Impls.Num(), false, "Empty steps!")

	/**
	*	PostCondition will modify Infos.
	*	PostCondition must be added to check next PreConditions.
	*	Call ResetPostCondition to get back initial Infos.
	*	Avoid Infos copy.
	*/
	USP_PlanGenInfos* NonConstPlanGenInfos = const_cast<USP_PlanGenInfos*>(Infos);

	for (int i = 0; i < Impls.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Impls[i], false)

		if (!Impls[i]->PreCondition(Infos) || !Impls[i]->PostCondition(NonConstPlanGenInfos))
		{
			// PostCondition failed: Reset all previously set.
		
			for (int j = i - 1; j >= 0; --j)
				Impls[j]->ResetPostCondition(NonConstPlanGenInfos);

			return false;
		}
	}


	// Reset NonConstPlanGenInfos to initial value.
	for (int i = 0; i < Impls.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Impls[i], false)

		Impls[i]->ResetPostCondition(NonConstPlanGenInfos);
	}

	return true;
}
bool USP_ActionImplChain::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	SP_RCHECK(Impls.Num(), false, "Empty steps!")

	for (int i = 0; i < Impls.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Impls[i], false)

		// Precondition has already been checked.
		if (!Impls[i]->PostCondition(Infos))
		{
			// Post Condition failed: Reset all previously set.

			for (int j = i - 1; j >= 0; --j)
				Impls[j]->ResetPostCondition(Infos);

			return false;
		}
	}

	return true;
}

bool USP_ActionImplChain::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	SP_RCHECK(Impls.Num(), false, "Empty steps!")

	for (int i = 0; i < Impls.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Impls[i], false)

		Impls[i]->ResetPostCondition(Infos);
	}

	return true;
}