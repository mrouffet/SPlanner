#include <SPlanner/Actions/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStep::PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}
uint64 USP_ActionStep::PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, PlannerFlags)

	return PlannerFlags;
}