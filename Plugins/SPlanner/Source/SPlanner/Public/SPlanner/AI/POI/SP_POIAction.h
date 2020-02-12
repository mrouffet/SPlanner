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
	TArray<USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> AchievedGoals;

public:
	/** Getter of Task. */
	USP_Task* GetTask() const;

	/** Getter of ServedGoals. */
	const TArray<USP_Goal*>& GetServedGoals() const;

	/** Getter of AchievedGoals. */
	const TArray<USP_Goal*>& GetAchievedGoals() const;
};