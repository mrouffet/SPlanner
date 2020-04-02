// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleTask.h>

float USP_IdleTask::GenerateTime_Implementation(const USP_AIPlannerComponent* Planner)
{
	return FMath::RandRange(MinTime, MaxTime);
}