// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

#include <UObject/Object.h>
#include "SP_PlanGenInfos.generated.h"

class USP_ActionImpl;

/**
*	Implementation of structure infos for plan generation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Planner")
class SPLANNER_API USP_PlanGenInfos : public UObject
{
	GENERATED_BODY()

public:
	/** Max planner depth. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	int MaxDepth = -1;

	/** Associated planner for generation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	/** Available Actions for generation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	FSP_PlannerActionSet PlannerActions;

	/** Output plan after generation. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_ActionImpl*> OutPlan;

	/** Setter of MaxDepth and Reserve OutPlan. */
	void SetMaxDepth(int NewMaxDepth);
};