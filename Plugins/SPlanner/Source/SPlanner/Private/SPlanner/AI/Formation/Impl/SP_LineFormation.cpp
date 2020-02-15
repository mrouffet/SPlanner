// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_LineFormation.h>

USP_LineFormation::USP_LineFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 3;
}

void USP_LineFormation::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}