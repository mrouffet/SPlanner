// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

void USP_PlanGenInfos::SetMaxDepth(int NewMaxDepth)
{
	MaxDepth = NewMaxDepth;

	OutPlan.Reserve(MaxDepth);
}