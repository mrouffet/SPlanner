// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

#include <SPlanner/Debug/SP_Debug.h>

float USP_FloatParam::Query_Implementation(const UObject* Outer) const
{
	float Value = DefaultValue;

	// Compute average inputs.
	float Input = 0.0f;

	// Multiply by average input.
	if (Value != 0.0f && Inputs.Num())
	{
		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Inputs[i])

			Input += Inputs[i]->Get();
		}
	}

	return Value + Input / Inputs.Num();
}