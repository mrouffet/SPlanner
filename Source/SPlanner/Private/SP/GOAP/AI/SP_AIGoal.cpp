// Copyright 2023 Maxime ROUFFET. All Rights Reserved.

#include <SP/GOAP/AI/SP_AIGoal.h>

void OnStart_Implementation(USP_AIGOAPComponent* Planner)
{
	Planner.Add(Planner);
}

void OnEnd_Implementation(USP_AIGOAPComponent* Planner)
{
	Planner.Remove(Planner);
}
