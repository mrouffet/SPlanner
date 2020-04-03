// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleCurveTask.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatCurveParam.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

float USP_IdleCurveTask::GenerateTime_Implementation(const USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(MinCurve, -1.0f)
	SP_RCHECK_NULLPTR(MaxCurve, -1.0f)

	float MinTime = MinCurve->Query(Planner);
	float MaxTime = MaxCurve->Query(Planner);

	SP_RCHECK(MinTime > 0.0f || MaxTime > 0.0f, 0.0f, "Invalid arguments: both MinTime and MaxTime are < 0.0f!")

	if (MinTime < 0.0f)
		return MaxTime;
	else if (MaxTime < 0.0f)
		return MinTime;

	return FMath::RandRange(MinTime, MaxTime);
}