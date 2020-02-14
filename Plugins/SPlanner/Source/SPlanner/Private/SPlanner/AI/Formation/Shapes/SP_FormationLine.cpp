// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Shapes/SP_FormationLine.h>

USP_FormationLine::USP_FormationLine(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 3;
}

void USP_FormationLine::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
}