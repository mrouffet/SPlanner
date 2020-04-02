// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Goal/SP_Goal.h>
#include "SP_AIGoal.generated.h"

class USP_IntAsset;
class USP_FloatAsset;

/**
 *	Planner goal asset.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Goal|AI")
class SPLANNER_API USP_AIGoal : public USP_Goal
{
	GENERATED_BODY()

protected:
	/** Whether blackboard must be reset when planner start this goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	bool bResetBlackboard = true;

	/**
	*	Minimum planner executing this goal.
	*	Use < 0 for unlimited.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	int MinPlannerNum = -1;

	/**
	*	Maximum planner executing this goal.
	*	Use < 0 for unlimited.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	int MaxPlannerNum = -1;

	/**
	*	Output Planners.Num() into a SP_IntAsset.
	*	Set nullptr to not use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	USP_IntAsset* OutputPlannerNum = nullptr;

	/**
	*	Output planner num / MaxPlannerNum ratio into a SP_FloatAsset.
	*	Require MaxPlannerNum > 0.
	*	Set nullptr to not use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal|AI")
	USP_FloatAsset* OutputMaxRatio = nullptr;

	bool CanStart_Implementation(const USP_PlannerComponent* Planner) const override;
	bool CanEnd_Implementation(const USP_PlannerComponent* Planner) const override;

	void OnStart_Implementation(USP_PlannerComponent* InPlanner) override;
	void OnEnd_Implementation(USP_PlannerComponent* InPlanner) override;

public:
	/** Getter of MinPlannerNum*/
	float GetMinPlannerNum() const;

	/** Getter of MaxPlannerNum*/
	float GetMaxPlannerNum() const;
};