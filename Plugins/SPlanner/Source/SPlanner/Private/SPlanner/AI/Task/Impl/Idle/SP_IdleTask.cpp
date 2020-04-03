// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleTask.h>

#include <SPlanner/Debug/SP_Debug.h>

float USP_IdleTask::GenerateTime_Implementation(const USP_AIPlannerComponent* Planner)
{
	SP_RCHECK(MinTime > 0.0f || MaxTime > 0.0f, 0.0f, "Invalid arguments: both MinTime and MaxTime are < 0.0f!")

	if (MinTime < 0.0f)
		return MaxTime;
	else if (MaxTime < 0.0f)
		return MinTime;

	return FMath::RandRange(MinTime, MaxTime);
}