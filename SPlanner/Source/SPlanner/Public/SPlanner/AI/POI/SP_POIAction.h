// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Actions/SP_Action.h>
#include "SP_POIAction.generated.h"

class USP_Task;
class USP_Goal;

/**
*	Planner POI Action.
*	Pair task and weight.
*/
USTRUCT(BlueprintType, Category = "SPlanner|POI")
struct FSP_POIAction : public FSP_Action
{
	GENERATED_BODY()

protected:
	/** All goals served by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<const USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<const USP_Goal*> AchievedGoals;

public:
	/** Getter of Task. */
	USP_Task* GetTask() const;

	/** Whether Goal is in AchievedGoals */
	bool IsGoalAchieved(const USP_Goal* Goal) const;

	bool CheckAvailability(const USP_PlannerComponent* Planner) const override;
};