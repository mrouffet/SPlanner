// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>
#include "SP_FormationRectangle.generated.h"

/**
 *	Rectangle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation|Shape")
class SPLANNER_API USP_FormationRectangle : public USP_FormationShape
{
	GENERATED_BODY()

protected:
	/** The width of the rectangle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Rectangle")
	float Width = 100.0f;

	/** The heght of the rectangle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Rectangle")
	float Height = 100.0f;

public:
	USP_FormationRectangle(const FObjectInitializer& ObjectInitializer);

	void Compute(const TArray<USP_AIPlannerComponent*>& Planners) override;
};