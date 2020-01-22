#include <SPlanner/Actions/SP_ActionStepChain.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStepChain::PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	SP_RCHECK(Steps.Num(), "Empty steps!", false)

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		if (!Steps[i]->PreCondition(Planner, PlannerFlags))
			return false;

		PlannerFlags = Steps[i]->PostCondition(Planner, PlannerFlags);
	}

	return true;
}
uint64 USP_ActionStepChain::PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_RCHECK(Steps.Num(), "Empty steps!", PlannerFlags)

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], PlannerFlags)

		// Precondition has already been checked.
		PlannerFlags = Steps[i]->PostCondition(Planner, PlannerFlags);
	}

	return PlannerFlags;
}