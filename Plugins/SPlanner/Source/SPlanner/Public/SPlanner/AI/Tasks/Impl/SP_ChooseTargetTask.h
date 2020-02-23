// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_ChooseTargetTask.generated.h"

/** The method to use to choose a target. */
UENUM(BlueprintType)
enum class ESP_ChooseTargetMethod : uint8
{
	/** Target the nearest. */
	CTM_Nearest				UMETA(DisplayName = "Nearest"),

	/** Target the furthest. */
	CTM_Furthest			UMETA(DisplayName = "Furthest"),

	/** Target random. */
	CTM_Random				UMETA(DisplayName = "Random")
};

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

	/** The method of selection of Target from list. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	ESP_ChooseTargetMethod Method = ESP_ChooseTargetMethod::CTM_Nearest;

	/** Local offset to apply to the center of the field of view. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector LocalOffset = FVector::ZeroVector;

	/**
	*	Dimensions of field of view.
	*	Set < 0.0f for unlimited bound.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FVector HalfDimensions = FVector(-1.0f, -1.0f, -1.0f);

	/** Predicate to validate to choose an Actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Action|Task|Target")
	bool Predicate(const USP_AIPlannerComponent* Planner, const AActor* Actor) const;

	/** Choose target from list using Method. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Action|Task|Target")
	AActor* Choose(const USP_AIPlannerComponent* Planner, const TArray<AActor*>& Actors);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	FColor DebugColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "SPlanner|DEBUG")
	float DebugDrawTime = 2.0f;
#endif

#if WITH_EDITOR
	void DrawDebug(const USP_AIPlannerComponent* Planner, const FVector& TargetLocation);
#endif

public:
	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;

	UFUNCTION(BlueprintPure, Category = "SPlanner|Action|Task|Target")
	FVector GetFOVCenter(const APawn* Pawn) const;
};