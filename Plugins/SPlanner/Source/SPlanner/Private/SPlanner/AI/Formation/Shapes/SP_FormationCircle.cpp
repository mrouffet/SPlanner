// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Shapes/SP_FormationCircle.h>

USP_FormationCircle::USP_FormationCircle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 5;
}

void USP_FormationCircle::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}