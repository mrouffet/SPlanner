#include <SPlanner/Goals/SP_SequenceConditionGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Goals/SP_ConditionGoal.h>

const USP_Goal* USP_SequenceConditionGoal::GetExecutionGoal(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	for(int i = 0; i < SubGoals.Num(); ++i)
	{
		SP_CCHECK(SubGoals[i], "SubGoals [%d] in [%s] is nullptr!", i, *GetName())

		if (SubGoals[i]->Predicate(Planner))
			return SubGoals[i];
	}

	SP_LOG(Warning, "No sub-goal found!")

	return this;
}