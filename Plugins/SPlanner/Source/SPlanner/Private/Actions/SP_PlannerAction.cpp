#include <Actions/SP_PlannerAction.h>

FSP_PlannerAction::FSP_PlannerAction(USP_ActionStep* InStep, float InWeight, bool bInAchievePlan) :
	FSP_Action(InStep, InWeight),
	bAchievePlan { bInAchievePlan }
{
}