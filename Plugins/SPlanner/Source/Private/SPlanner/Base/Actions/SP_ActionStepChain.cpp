#include <SPlanner/Base/Actions/SP_ActionStepChain.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStepChain::PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	SP_RCHECK(Steps.Num(), false, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], false)

		if (!Steps[i]->PreCondition(Planner, GeneratedPlan, PlannerFlags))
			return false;

		PlannerFlags = Steps[i]->PostCondition(Planner, PlannerFlags);
	}

	return true;
}
uint64 USP_ActionStepChain::PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_RCHECK(Steps.Num(), PlannerFlags, "Empty steps!")

	for (int i = 0; i < Steps.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Steps[i], PlannerFlags)

		// Precondition has already been checked.
		PlannerFlags = Steps[i]->PostCondition(Planner, PlannerFlags);
	}

	return PlannerFlags;
}