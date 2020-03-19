// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/AI/Tasks/SP_Task.h>

USP_Task* FSP_POIAction::GetTask() const
{
	SP_SRCHECK_NULLPTR(Cast<USP_Task>(Step), nullptr)

	return Cast<USP_Task>(Step);
}

bool FSP_POIAction::IsGoalAchieved(const USP_Goal* Goal) const
{
	SP_SRCHECK_NULLPTR(Goal, false)

	return AchievedGoals.Find(Goal) != INDEX_NONE;
}

bool FSP_POIAction::CheckAvailability(const USP_PlannerComponent* Planner) const
{
	if (!Super::CheckAvailability(Planner))
		return false;

	return ServedGoals.Find(Planner->GetGoal()) != INDEX_NONE || AchievedGoals.Find(Planner->GetGoal()) != INDEX_NONE;
}