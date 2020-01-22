#pragma once
#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Components/Zones/SP_LODComponent.h>
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

	/**
	*	Getter of MaxPlannerDepth using MaxPlannerDepthCurve and LODLevel.
	*	Eval LODLevel using GetLevel() if LODLevel < 0.0f.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	int GetMaxPlannerDepth(float LODLevel = -1.0f) const;

	/**
	*	Getter of  TimeBeforeConstructPlan using  TimeBeforeConstructPlanCurve and LODLevel.
	*	Eval LODLevel using GetLevel() if LODLevel < 0.0f.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	float GetTimeBeforeConstructPlan(float LODLevel = -1.0f) const;
};