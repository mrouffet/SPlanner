// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionSet.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_PlannerActionSet.h>

const TArray<FSP_Action>& USP_ActionSet::GetBeginActions() const
{
	return BeginActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetCoreActions() const
{
	return CoreActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetForcedActions() const
{
	return ForcedActions;
}
const TArray<FSP_Action>& USP_ActionSet::GetEndActions() const
{
	return EndActions;
}

FSP_PlannerActionSet USP_ActionSet::Shuffle(const USP_PlannerComponent* Planner, float LODLevel, bool* bCanBeAchievedPtr) const
{
	SP_RCHECK_NULLPTR(Planner, FSP_PlannerActionSet())
	SP_RCHECK(EndActions.Num(), FSP_PlannerActionSet(), "No EndActions: Can't generate a valid plan!")

	return FSP_PlannerActionSet::Make(this, Planner, LODLevel, bCanBeAchievedPtr);
}