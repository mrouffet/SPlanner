// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Base/LOD/SP_LODComponent.h>
#include "SP_AILODComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, DisplayName = "SP_AILODComponent", ClassGroup = "SPlanner|LOD|AI", meta = (BlueprintSpawnableComponent))
class SPLANNER_API USP_AILODComponent : public USP_LODComponent
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
	USP_AILODComponent(const FObjectInitializer& ObjectInitializer);

	/**
	*	Getter of MaxPlannerDepth using MaxPlannerDepthCurve and LODLevel.
	*	Eval LODLevel using GetLevel() if LODLevel < 0.0f.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD|AI")
	int GetMaxPlannerDepth() const;

	/**
	*	Getter of  TimeBeforeConstructPlan using  TimeBeforeConstructPlanCurve and LODLevel.
	*	Eval LODLevel using GetLevel() if LODLevel < 0.0f.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD|AI")
	float GetTimeBeforeConstructPlan() const;
};