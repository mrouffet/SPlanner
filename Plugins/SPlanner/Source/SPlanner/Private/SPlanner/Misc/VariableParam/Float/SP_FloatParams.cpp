// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/Float/SP_FloatParams.h>

#include <SPlanner/Debug/SP_Debug.h>

float USP_FloatParams::Query_Implementation(const UObject* Outer) const
{
	float Value = Super::Query_Implementation(Outer);

	float TotalInput = 0.0f;

	if (Inputs.Num())
	{
		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Inputs[i])

			TotalInput += Inputs[i]->QueryFloat(Outer);
		}

		// Compute average inputs.
		if (bUseAverage)
			TotalInput /= Inputs.Num();
	}

	return Value + TotalInput;
}