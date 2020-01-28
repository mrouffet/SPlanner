#include <SPlanner/Goals/SP_ConditionGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ConditionGoal::Predicate(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}

const USP_Goal* USP_ConditionGoal::GetExecutionGoal(const USP_PlannerComponent* Planner) const
{
	if (Predicate(Planner))
		return this;

	return nullptr;
}