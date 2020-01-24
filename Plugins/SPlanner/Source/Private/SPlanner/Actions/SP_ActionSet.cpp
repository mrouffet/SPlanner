#include <SPlanner/Actions/SP_ActionSet.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Actions/SP_PlannerActionSet.h>

const TArray<FSP_Action>& USP_ActionSet::GetBeginActions() const
{
	return BeginActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetCoreActions() const
{
	return CoreActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetForcedCoreActions() const
{
	return ForcedCoreActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetEndActions() const
{
	return EndActions;
}

FSP_PlannerActionSet USP_ActionSet::Shuffle(float LODLevel) const
{
	SP_RCHECK(EndActions.Num(), FSP_PlannerActionSet(), "No EndActions: Can't generate a valid plan!")

	return FSP_PlannerActionSet::Make(this, LODLevel);
}