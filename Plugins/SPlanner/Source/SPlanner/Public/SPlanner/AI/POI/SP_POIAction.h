// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_Action.h>
#include "SP_POIAction.generated.h"

class USP_TaskStep;
class USP_Goal;

/**
*	Planner POI Action.
*	Pair task and weight.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, Category = "SPlanner|POI")
class USP_POIAction : public USP_Action
{
	GENERATED_BODY()

protected:
	/** All goals served by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<const USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<const USP_Goal*> AchievedGoals;

public:
	/** Getter of Task. */
	USP_TaskStep* GetTask() const;

	/** Whether Goal is in AchievedGoals */
	bool IsGoalAchieved(const USP_Goal* Goal) const;

	bool IsAvailable(const USP_PlannerComponent* Planner) const override;
};