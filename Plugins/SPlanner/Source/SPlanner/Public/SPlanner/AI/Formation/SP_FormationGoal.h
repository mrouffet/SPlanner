// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Goal/SP_Goal.h>
#include "SP_FormationGoal.generated.h"

class USP_FormationSet;

/**
 *	AI Planner goal with formation asset.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI|Goal")
class SPLANNER_API USP_FormationGoal : public USP_Goal
{
	GENERATED_BODY()

protected:
	/** The formation to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	USP_FormationSet* FormationSet = nullptr;

	void OnStart_Implementation(USP_PlannerComponent* Planner) override;
	void OnEnd_Implementation(USP_PlannerComponent* Planner) override;
};