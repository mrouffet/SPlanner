// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_HalfCircleFormation.h>

USP_HalfCircleFormation::USP_HalfCircleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 5;
}

void USP_HalfCircleFormation::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}