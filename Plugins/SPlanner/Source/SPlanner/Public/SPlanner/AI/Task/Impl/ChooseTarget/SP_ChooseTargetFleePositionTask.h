// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetFleePositionTask.generated.h"

/**
 *	Choose target implementation with flee spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetFleePositionTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object or position to flee in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName FleeTargetEntryName = "None";

	/** The maximum raycast number to find a flee point. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	int MaxRaycastNum = 5;

	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	FVector ComputeFleePoint(const FVector& FleeDirection, const FVector& FOVCenter) const;

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;

	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_ChooseTargetFleePositionTask(const FObjectInitializer& ObjectInitializer);
};