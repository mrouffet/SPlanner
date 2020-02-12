#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/Debug/SP_Debug.h>

float FSP_FloatParam::Get(float LODLevel) const
{
	float Value = -1.0f;

	if (LODLevel < 0.0f || LODCurve.GetRichCurveConst()->IsEmpty())
		Value = Default;
	else
		Value = LODCurve.GetRichCurveConst()->Eval(LODLevel);

	// Multiply by average input.
	if (Value != 0.0f && Inputs.Num())
	{
		float Input = 0.0f;

		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_SCCHECK_NULLPTR(Inputs[i])

			Input += Inputs[i]->Get();
		}

		Value *= Input / Inputs.Num();
	}

	return Value;
}