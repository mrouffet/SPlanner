// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/SP_Formation.h>
#include "SP_RectangleFormation.generated.h"

/**
 *	Rectangle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_RectangleFormation : public USP_Formation
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
	USP_RectangleFormation(const FObjectInitializer& ObjectInitializer);
};