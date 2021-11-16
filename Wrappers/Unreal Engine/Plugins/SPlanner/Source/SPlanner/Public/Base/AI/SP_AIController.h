// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <AIController.h>

#include "SP_AIController.generated.h"

class USP_AIPlannerComponent;
class USP_AIPlannerAsset;

UCLASS(ClassGroup = "SPlanner|AI")
class SPLANNER_API ASP_AIController : public AAIController
{
	GENERATED_BODY()

public:
	/** Component responsible for planner behaviors. */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	USP_AIPlannerComponent* PlannerComponent = nullptr;

	ASP_AIController(const FObjectInitializer& ObjectInitializer);

	/** Starts executing planner behavior. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual bool RunPlanner(USP_AIPlannerAsset* SPAsset);
};
