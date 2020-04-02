// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleCurveTask.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatCurveParam.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

float USP_IdleCurveTask::GenerateTime_Implementation(const USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(MinCurve, -1.0f)
	SP_RCHECK_NULLPTR(MaxCurve, -1.0f)

	return FMath::RandRange(MinCurve->Query(Planner), MinCurve->Query(MaxCurve));
}