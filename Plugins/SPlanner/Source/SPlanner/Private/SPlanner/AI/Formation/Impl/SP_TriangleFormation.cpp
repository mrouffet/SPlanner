// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_TriangleFormation.h>

USP_TriangleFormation::USP_TriangleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 3;
}

void USP_TriangleFormation::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}