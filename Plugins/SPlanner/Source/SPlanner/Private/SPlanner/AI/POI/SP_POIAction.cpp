// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/AI/Task/SP_TaskImpl.h>

USP_TaskImpl* USP_POIAction::GetTaskImpl() const
{
	SP_SRCHECK_NULLPTR(Cast<USP_TaskImpl>(Impl), nullptr)

	return Cast<USP_TaskImpl>(Impl);
}

bool USP_POIAction::IsGoalAchieved(const USP_Goal* Goal) const
{
	SP_SRCHECK_NULLPTR(Goal, false)

	return AchievedGoals.Find(Goal) != INDEX_NONE;
}

bool USP_POIAction::IsAvailable(const USP_PlannerComponent* Planner) const
{
	if (!Super::IsAvailable(Planner))
		return false;

	return ServedGoals.Find(Planner->GetGoal()) != INDEX_NONE || AchievedGoals.Find(Planner->GetGoal()) != INDEX_NONE;
}