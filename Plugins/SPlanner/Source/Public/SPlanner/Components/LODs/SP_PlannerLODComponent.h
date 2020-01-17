#pragma once
#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Components/LODs/SP_LODComponent.h>
#include "SP_PlannerLODComponent.generated.h"

class USP_PlannerComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|LOD")
class SPLANNER_API USP_PlannerLODComponent : public USP_LODComponent
{
	GENERATED_BODY()

protected:
	/**
	*	The maximum planner depth while building a plan.
	*	Plan generation complexity: O(!n), n = MaxPlannerDepth.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve MaxPlannerDepthCurve;

	/**
	*	Minimum time to wait before constructing a new plan.
	*	Use <= 0 to compute instantly.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve TimeBeforeConstructPlanCurve;

public:
	USP_PlannerLODComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of MaxPlannerDepth using MaxPlannerDepthCurve */
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	int GetMaxPlannerDepth(USP_PlannerComponent* Planner) const;

	/** Getter of MaxPlannerDepth using MaxPlannerDepthCurve */
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	float GetTimeBeforeConstructPlan(USP_PlannerComponent* Planner) const;
};