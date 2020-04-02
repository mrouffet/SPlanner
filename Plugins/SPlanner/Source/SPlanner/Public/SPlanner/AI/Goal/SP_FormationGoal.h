// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Goal/SP_AIGoal.h>
#include "SP_FormationGoal.generated.h"

class USP_FormationSet;

/**
 *	AI Planner goal with formation asset.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|AI")
class SPLANNER_API USP_FormationGoal : public USP_AIGoal
{
	GENERATED_BODY()

protected:
	/** The formation to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI|Formation")
	USP_FormationSet* FormationSet = nullptr;

	bool CanStart_Implementation(const USP_PlannerComponent* Planner) const override;
	bool CanEnd_Implementation(const USP_PlannerComponent* Planner) const override;

	void OnStart_Implementation(USP_PlannerComponent* Planner) override;
	void OnEnd_Implementation(USP_PlannerComponent* Planner) override;
};