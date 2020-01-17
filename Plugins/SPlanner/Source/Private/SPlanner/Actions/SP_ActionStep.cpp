#include <SPlanner/Actions/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ActionStep::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}
FSP_PlannerFlags USP_ActionStep::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, PlannerFlags)

	return PlannerFlags;
}