// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>
#include "SP_FormationLine.generated.h"

/**
 *	Line shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation|Shape")
class SPLANNER_API USP_FormationLine : public USP_FormationShape
{
	GENERATED_BODY()
	
protected:
	/** The width of the line. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Line")
	float Width = 300.0f;

	/** The width of the line. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Line")
	float DistToLead = 100.0f;

public:
	USP_FormationLine(const FObjectInitializer& ObjectInitializer);

	void Compute(const TArray<USP_AIPlannerComponent*>& Planners) override;
};