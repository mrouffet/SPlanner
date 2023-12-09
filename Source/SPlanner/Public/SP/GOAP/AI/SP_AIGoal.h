// Copyright 2023 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SP/GOAP/Base/SP_Goal.h>
#include "SP_AIGoal.generated.h"

class USP_AIGOAPComponent;

/**
*	SPlanner GOAP goal AI implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI|GOAP")
class SPLANNER_API USP_AIGoal : public USP_Goal
{
	GENERATED_BODY()

protected:
	/** All planners registered currently executing this goal. */
	TArray<USP_AIGOAPComponent*> Planners;

public:
	/** Getter of Planners. */
	const TArray<USP_AIGOAPComponent*>& GetPlanners() const;

	/** Callback method called when Planner starts this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|AI|GOAP")
	void OnStart(USP_AIGOAPComponent* Planner);

	/** Callback method called when Planner ends this goal. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|AI|GOAP")
	void OnEnd(USP_AIGOAPComponent* Planner);
};
