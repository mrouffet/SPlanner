#include <SPlanner/Components/LODs/SP_PlannerLODComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Components/Planners/SP_PlannerComponent.h>

USP_PlannerLODComponent::USP_PlannerLODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FRichCurve* const MaxPlannerDepthCurve_Internal = MaxPlannerDepthCurve.GetRichCurve();
	MaxPlannerDepthCurve_Internal->AddKey(1000.0f, 5.0f);
	MaxPlannerDepthCurve_Internal->AddKey(2000.0f, 3.0f);
	MaxPlannerDepthCurve_Internal->AddKey(5000.0f, 2.0f);


	FRichCurve* const TimeBeforeConstructPlanCurve_Internal = TimeBeforeConstructPlanCurve.GetRichCurve();
	TimeBeforeConstructPlanCurve_Internal->AddKey(1000.0f, -1.0f);
	TimeBeforeConstructPlanCurve_Internal->AddKey(2000.0f, 5.0f);
	TimeBeforeConstructPlanCurve_Internal->AddKey(5000.0f, 10.0f);
}

int USP_PlannerLODComponent::GetMaxPlannerDepth(USP_PlannerComponent* Planner) const
{
	return static_cast<int>(MaxPlannerDepthCurve.GetRichCurveConst()->Eval(GetDistanceFromCamera(Planner)));
}
float USP_PlannerLODComponent::GetTimeBeforeConstructPlan(USP_PlannerComponent* Planner) const
{
	return TimeBeforeConstructPlanCurve.GetRichCurveConst()->Eval(GetDistanceFromCamera(Planner));
}