// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_ChooseTargetTask.generated.h"

/**
 *	Base implementation of ChooseTarget task.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetTask : public USP_Task
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName TargetEntryName = "None";

	/** Can target again even if a previous target has been set in plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bAllowReTarget = false;

	/** Whether the target should be visible. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bTargetVisible = true;

	/** Local offset to apply to the center of the field of view. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector LocalOffset = FVector::ZeroVector;

	/**
	*	Dimensions of minimum field of view.
	*	Set < 0.0f for unlimited bound.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector MinDimensions = FVector(-1.0f, -1.0f, -1.0f);

	/**
	*	Dimensions of maximum field of view.
	*	Set < 0.0f for unlimited bound.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector MaxDimensions = FVector(-1.0f, -1.0f, -1.0f);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor MinFOVDebugColor = FColor::Orange;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor MaxFOVDebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor LineDebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 2.0f;
#endif

	bool PreCondition_Implementation(const USP_PlannerComponent* Planner,
		const TArray<USP_ActionStep*>& GeneratedPlan,
		const USP_PlanGenInfos* PlanGenInfos) const override;
	bool PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const override;
	bool ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const override;

#if WITH_EDITOR
	void DrawDebug(const USP_AIPlannerComponent* Planner, const FVector& TargetLocation);
#endif

public:
	/** Get the FOV center using Pawn and LocalOffset. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Action|Task|Target")
	FVector GetFOVCenter(const APawn* Pawn) const;
};