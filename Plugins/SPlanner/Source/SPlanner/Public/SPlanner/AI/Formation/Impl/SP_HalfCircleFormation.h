// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/SP_Formation.h>
#include "SP_HalfCircleFormation.generated.h"

/**
 *	Circle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_HalfCircleFormation : public USP_Formation
{
	GENERATED_BODY()

protected:
	/** The radius of the circle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|HalfCircle")
	float Radius = 100.0f;

public:
	USP_HalfCircleFormation(const FObjectInitializer& ObjectInitializer);
};