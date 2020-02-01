// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/SP_Formation.h>
#include "SP_LineFormation.generated.h"

/**
 *	Line shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_LineFormation : public USP_Formation
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
	USP_LineFormation(const FObjectInitializer& ObjectInitializer);
};