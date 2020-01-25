#include <SPlanner/Miscs/SP_Weight.h>

#include <SPlanner/Debug/SP_Debug.h>

float FSP_Weight::Get(float LODLevel) const
{
	float Weight = -1.0f;

	if (LODLevel < 0.0f || WeightLODCurve.GetRichCurveConst()->IsEmpty())
		Weight = DefaultWeight;
	else
		Weight = WeightLODCurve.GetRichCurveConst()->Eval(LODLevel);

	for (int i = 0; i < InputWeights.Num(); ++i)
	{
		SP_SCCHECK_NULLPTR(InputWeights[i])

		Weight *= InputWeights[i]->Get();
	}

	return Weight;
}