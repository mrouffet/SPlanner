#include <SPlanner/Actions/SP_Action.h>

float FSP_Action::GetWeight(float LODLevel) const
{
	if (LODLevel < 0.0f || WeightLODCurve.GetRichCurveConst()->IsEmpty())
		return DefaultWeight;

	return WeightLODCurve.GetRichCurveConst()->Eval(LODLevel);
}