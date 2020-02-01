// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Formation/SP_Formation.h>
#include "SP_CircleFormation.generated.h"

/**
 *	Circle shape formation implementation.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "SPlanner|AI|Formation")
class SPLANNER_API USP_CircleFormation : public USP_Formation
{
	GENERATED_BODY()

protected:
	/** The radius of the circle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Circle")
	float Radius = 100.0f;

	void ConstructDichotomy(FSP_FormationInfos& Infos) override;
	void ConstructPointByPoint(FSP_FormationInfos& Infos) override;

public:
	USP_CircleFormation(const FObjectInitializer& ObjectInitializer);
};