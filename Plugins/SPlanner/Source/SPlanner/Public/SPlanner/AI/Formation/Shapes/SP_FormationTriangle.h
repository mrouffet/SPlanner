// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>
#include "SP_FormationTriangle.generated.h"

/**
 *	Triangle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation|Shape")
class SPLANNER_API USP_FormationTriangle : public USP_FormationShape
{
	GENERATED_BODY()
	
public:
	USP_FormationTriangle(const FObjectInitializer& ObjectInitializer);
};