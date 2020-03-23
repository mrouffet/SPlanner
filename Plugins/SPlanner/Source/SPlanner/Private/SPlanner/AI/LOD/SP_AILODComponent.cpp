// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/LOD/SP_AILODComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_AILODComponent::USP_AILODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FRichCurve* const MaxPlannerDepthCurve_Internal = MaxPlannerDepthCurve.GetRichCurve();
	MaxPlannerDepthCurve_Internal->AddKey(2500.0f, 5.0f);
	MaxPlannerDepthCurve_Internal->AddKey(5000.0f, 3.0f);
	MaxPlannerDepthCurve_Internal->AddKey(10000.0f, 2.0f);


	FRichCurve* const TimeBeforeConstructPlanCurve_Internal = TimeBeforeConstructPlanCurve.GetRichCurve();
	TimeBeforeConstructPlanCurve_Internal->AddKey(2500.0f, -1.0f);
	TimeBeforeConstructPlanCurve_Internal->AddKey(5000.0f, 5.0f);
	TimeBeforeConstructPlanCurve_Internal->AddKey(10000.0f, 10.0f);
}

int USP_AILODComponent::GetMaxPlannerDepth() const
{
	return static_cast<int>(MaxPlannerDepthCurve.GetRichCurveConst()->Eval(LODLevel));
}
float USP_AILODComponent::GetTimeBeforeConstructPlan() const
{
	return TimeBeforeConstructPlanCurve.GetRichCurveConst()->Eval(LODLevel);
}