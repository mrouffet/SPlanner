// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_CircleFormation.h>

USP_CircleFormation::USP_CircleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 10;
}

void USP_CircleFormation::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}