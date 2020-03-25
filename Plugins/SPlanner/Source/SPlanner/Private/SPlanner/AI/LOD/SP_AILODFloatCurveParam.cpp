// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/LOD/SP_AILODFloatCurveParam.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

float USP_AILODFloatCurveParam::Query_Implementation(const UObject* Outer) const
{
	float Value = -1.0f;

	if (const USP_AIPlannerComponent* const Planner = Cast<USP_AIPlannerComponent>(Outer))
	{
		// Create Infos.
		USP_FloatCurveParamInfos* const Infos = NewObject<USP_FloatCurveParamInfos>();

		Infos->Outer = Planner;
		Infos->CurveTime = Planner->GetLODLevel();

		Value = Super::Query_Implementation(Infos);
	}
	else
	{
		SP_LOG(Warning, "Outer is not of type USP_FloatCurveParamInfos")

		Value = Super::Query_Implementation(Outer);
	}

	return Value;
}