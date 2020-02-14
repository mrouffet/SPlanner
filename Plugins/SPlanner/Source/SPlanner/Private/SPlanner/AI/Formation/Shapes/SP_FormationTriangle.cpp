// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Shapes/SP_FormationTriangle.h>

USP_FormationTriangle::USP_FormationTriangle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 3;
}

void USP_FormationTriangle::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}