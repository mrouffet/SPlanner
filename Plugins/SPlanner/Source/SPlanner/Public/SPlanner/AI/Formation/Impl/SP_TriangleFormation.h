// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/SP_Formation.h>
#include "SP_TriangleFormation.generated.h"

/**
 *	Triangle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_TriangleFormation : public USP_Formation
{
	GENERATED_BODY()
	
protected:
	/** The base of the triangle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Triangle")
	float Base = 100.0f;

	/** The height of the triangle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Triangle")
	float Height = 100.0f;

public:
	USP_TriangleFormation(const FObjectInitializer& ObjectInitializer);
};