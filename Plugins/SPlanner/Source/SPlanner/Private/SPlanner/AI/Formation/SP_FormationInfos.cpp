// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationInfos.h>

#include <SPlanner/Debug/SP_Debug.h>

bool FSP_PlannerFormatioInfos::operator<(const FSP_PlannerFormatioInfos& Rhs) const
{
	return Angle < Rhs.Angle;
}


FSP_FormationInfos::FSP_FormationInfos(const FSP_FormationSetInfos& SetInfos, const FVector& Direction) :
	BaseDirection{ Direction },
	LeadActor{ SetInfos.LeadActor },
	TargetActor{ SetInfos.TargetActor }
{
	// Create default object to keep references (see FSP_PlannerFormatioInfos.Offset declaration in SP_FormationInfos.h).
	SetInfos.Offsets.SetNum(SetInfos.Planners.Num());

	// Fill PlannerInfos. Angle will be computed depending on the formation construction type.
	for (int i = 0; i < SetInfos.Planners.Num(); ++i)
	{
		SP_SCCHECK_NULLPTR(SetInfos.Planners[i])
		PlannerInfos.Add(FSP_PlannerFormatioInfos{ SetInfos.Planners[i], 0.0f, SetInfos.Offsets[i] });
	}
}