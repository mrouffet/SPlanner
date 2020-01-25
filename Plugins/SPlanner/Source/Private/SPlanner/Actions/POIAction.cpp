#include <SPlanner/Actions/SP_POIAction.h>

USP_AITask* FSP_POIAction::GetTask() const
{
	return Task;
}

const TArray<USP_Goal*>& FSP_POIAction::GetServedGoals() const
{
	return ServedGoals;
}
const TArray<USP_Goal*>& FSP_POIAction::GetAchievedGoals() const
{
	return AchievedGoals;
}