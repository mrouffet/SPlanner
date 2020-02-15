// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>
#include "SP_FormationCircle.generated.h"

/**
 *	Circle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation|Shape")
class SPLANNER_API USP_FormationCircle : public USP_FormationShape
{
	GENERATED_BODY()

protected:
	/** The radius of the circle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Rectangle")
	float Radius = 100.0f;

public:
	USP_FormationCircle(const FObjectInitializer& ObjectInitializer);

	void Compute(const TArray<USP_AIPlannerComponent*>& Planners) override;
};