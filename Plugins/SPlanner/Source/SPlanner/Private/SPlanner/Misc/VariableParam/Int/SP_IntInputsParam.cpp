// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/Int/SP_IntInputsParam.h>

#include <SPlanner/Debug/SP_Debug.h>

int USP_IntInputsParam::Query_Implementation(const UObject* Outer) const
{
	int Value = Super::Query_Implementation(Outer);

	int TotalInput = 0;

	if (Inputs.Num())
	{
		for (int i = 0; i < Inputs.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(Inputs[i])

			TotalInput += Inputs[i]->Get();
		}

		// Compute average inputs.
		if (bUseAverage)
			TotalInput /= Inputs.Num();
	}

	return Value + TotalInput;
}