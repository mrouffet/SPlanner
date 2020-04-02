// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/Int/SP_IntInputsParam.h>

#include <SPlanner/Debug/SP_Debug.h>

int USP_IntInputsParam::Query_Implementation(const UObject* Outer) const
{
	int Value = Super::Query_Implementation(Outer);

	// Compute average inputs.
	int AverageInput = 0.0f;

	// Multiply by average input.
	if (Inputs.Num())
	{
		int Input = 0.0f;

		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Inputs[i])

			Input += Inputs[i]->Get();
		}

		AverageInput = Input / Inputs.Num();
	}

	return Value + AverageInput;
}