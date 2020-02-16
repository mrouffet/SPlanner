// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationInfos.h>

bool FSP_PlannerFormatioInfos::operator<(const FSP_PlannerFormatioInfos& Rhs) const
{
	return Angle < Rhs.Angle;
}


FSP_FormationInfos::FSP_FormationInfos(const FSP_FormationSetInfos& SetInfos, const FVector& Direction) :
	BaseDirection{ Direction },
	LeadActor{ SetInfos.LeadActor },
	TargetActor{ SetInfos.TargetActor }
{
}