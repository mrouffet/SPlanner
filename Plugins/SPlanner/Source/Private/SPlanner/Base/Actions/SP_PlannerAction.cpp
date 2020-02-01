#include <SPlanner/Base/Actions/SP_PlannerAction.h>

#include <SPlanner/Debug/SP_Debug.h>

FSP_PlannerAction FSP_PlannerAction::Make(const FSP_Action* Action, float LODLevel, bool bInAchievePlan)
{
	SP_SRCHECK_NULLPTR(Action, FSP_PlannerAction())

	return FSP_PlannerAction{ Action, Action->Weight.Get(LODLevel) * FMath::FRand(), bInAchievePlan };
}