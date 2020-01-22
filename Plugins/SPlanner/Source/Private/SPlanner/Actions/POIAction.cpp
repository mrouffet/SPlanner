#include <SPlanner/Actions/SP_POIAction.h>

USP_Task* FSP_POIAction::GetTask() const
{
	return Task;
}

float FSP_POIAction::GetWeight(float LODLevel) const
{
	if (LODLevel < 0.0f || WeightLODCurve.GetRichCurveConst()->IsEmpty())
		return DefaultWeight;

	return WeightLODCurve.GetRichCurveConst()->Eval(LODLevel);
}

const TArray<USP_Goal*>& FSP_POIAction::GetServedGoals() const
{
	return ServedGoals;
}
const TArray<USP_Goal*>& FSP_POIAction::GetAchievedGoals() const
{
	return AchievedGoals;
}