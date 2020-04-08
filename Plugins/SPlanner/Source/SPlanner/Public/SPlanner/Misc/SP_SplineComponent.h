// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Components/SplineComponent.h>
#include "SP_SplineComponent.generated.h"

/**
 * SPlanner spline implementation.
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent), Category = "SPlanner|Spline")
class SPLANNER_API USP_SplineComponent : public USplineComponent
{
	GENERATED_BODY()

protected:
//	/** Noise to apply at each point. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
//	TArray<FVector> Noises = { FVector::ZeroVector, FVector::ZeroVector };
//
//	FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const override;
//
//#if WITH_EDITOR
//	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
//#endif

public:
	///** Generate a noise in local space. */
	//UFUNCTION(BlueprintCallable, Category = "SPlanner|Spline")
	//FVector GenerateNoiseAtPoint(int32 PointIndex) const;

	///** Generate a noise in local space. */
	//UFUNCTION(BlueprintCallable, Category = "SPlanner|Spline")
	//FVector GenerateNoiseAtTime(float Time) const;

	///** Get the location at spline point */
	//UFUNCTION(BlueprintCallable, Category = Spline)
	//FVector GetLocationAtSplinePointNoise(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace) const;

	///** Given a time from 0 to the spline duration, return the point in space where this puts you */
	//UFUNCTION(BlueprintCallable, Category=Spline)
	//FVector GetLocationAtTimeNoise(float Time, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseConstantVelocity = false) const;

	/** Get the distance between 2 points of the spline. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Spline")
	float GetDistanceBetweenPoints(int32 StartPointIndex, int32 EndPointIndex);
};