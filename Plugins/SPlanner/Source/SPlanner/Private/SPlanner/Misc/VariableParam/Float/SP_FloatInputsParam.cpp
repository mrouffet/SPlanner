// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/Float/SP_FloatInputsParam.h>

#include <SPlanner/Debug/SP_Debug.h>

float USP_FloatInputsParam::Query_Implementation(const UObject* Outer) const
{
	float Value = Super::Query_Implementation(Outer);

	// Compute average inputs.
	float AverageInput = 0.0f;

	// Multiply by average input.
	if (Inputs.Num())
	{
		float Input = 0.0f;

		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Inputs[i])

			Input += Inputs[i]->Get();
		}

		AverageInput = Input / Inputs.Num();
	}

	return Value + AverageInput;
}