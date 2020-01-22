#include <SPlanner/Actions/SP_ActionSet.h>

#include <SPlanner/Debug/SP_Debug.h>

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
	SP_RCHECK(EndActions.Num(), "No EndActions: Can't generate a valid plan!", FSP_PlannerActionSet())

	return FSP_PlannerActionSet::Make(this, LODLevel);
}