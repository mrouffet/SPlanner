// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Director/SP_Director.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

bool USP_Goal::GetResetBlackboard() const
{
	return bResetBlackboard;
}

const TArray<USP_PlannerComponent*>& USP_Goal::GetPlanners() const
{
	return Planners;
}

const TArray<USP_Goal*>& USP_Goal::GetAllowedTransitions() const
{
	return AllowedTransitions;
}
const TArray<USP_Goal*>& USP_Goal::GetBlockedTransitions() const
{
	return BlockedTransitions;
}

bool USP_Goal::CanTransitTo(USP_Goal* NewGoal) const
{
	// Allowed transition is empty: accept all non-blocked goals.
	if (!AllowedTransitions.Num())
		return BlockedTransitions.Find(NewGoal) == INDEX_NONE;

	if (AllowedTransitions.Find(NewGoal) != INDEX_NONE)
	{
		SP_RCHECK(BlockedTransitions.Find(NewGoal) == INDEX_NONE, false, "Goal [%s] is both allowed and blocked!")
		return true;
	}

	return false;
}

bool USP_Goal::OnStart_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_RCHECK_NULLPTR(InPlanner, false)
	SP_RCHECK(Planners.Find(InPlanner) == INDEX_NONE, false, "Planner [%s] already registered!", *InPlanner->GetOwner()->GetName())

	Planners.Add(InPlanner);

	if (Planners.Num() == 1)
		ASP_Director::RegisterGoal(this);

	return true;
}
bool USP_Goal::OnEnd_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_RCHECK_NULLPTR(InPlanner, false)
	SP_RCHECK(Planners.Find(InPlanner) != INDEX_NONE, false, "Planner [%s] not previously registered!", *InPlanner->GetOwner()->GetName())

	Planners.Remove(InPlanner);

	if (Planners.Num() == 0)
		ASP_Director::UnRegisterGoal(this);

	return true;
}

void USP_Goal::Reset_Implementation()
{
	Planners.Empty();
}