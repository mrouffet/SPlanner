// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/AI/Task/SP_TaskStep.h>

USP_TaskStep* USP_POIAction::GetTask() const
{
	SP_SRCHECK_NULLPTR(Cast<USP_TaskStep>(Step), nullptr)

	return Cast<USP_TaskStep>(Step);
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