// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Planner/SP_AIFloatParam.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

float USP_AIFloatParam::Query_Implementation(const UObject* Outer) const
{
	const USP_AIPlannerComponent* const Planner = Cast<USP_AIPlannerComponent>(Outer);
	SP_RCHECK(Planner, Super::Query_Implementation(Outer), "Planner nullptr! Outer must be of type USP_AIPlannerComponent.")

	float LODLevel = Planner->GetLODLevel();

	// LOD inactive or no curve.
	if (LODLevel < 0.0f || LODCurve.GetRichCurveConst()->IsEmpty())
		return Super::Query_Implementation(Outer);
	
	float Value = LODCurve.GetRichCurveConst()->Eval(LODLevel);

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