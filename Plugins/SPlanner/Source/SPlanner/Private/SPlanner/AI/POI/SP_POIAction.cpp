// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Tasks/SP_Task.h>

USP_Task* FSP_POIAction::GetTask() const
{
	SP_SRCHECK_NULLPTR(Cast<USP_Task>(Step), nullptr)

	return Cast<USP_Task>(Step);
}

const TArray<USP_Goal*>& FSP_POIAction::GetServedGoals() const
{
	return ServedGoals;
}
const TArray<USP_Goal*>& FSP_POIAction::GetAchievedGoals() const
{
	return AchievedGoals;
}