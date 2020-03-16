// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>
#include "SP_ChooseTargetActorTask.generated.h"

/** The method to use to choose a target. */
UENUM(BlueprintType)
enum class ESP_ChooseTargetActorMethod : uint8
{
	/** Target the nearest. */
	CTM_Nearest				UMETA(DisplayName = "Nearest"),

	/** Target the furthest. */
	CTM_Furthest			UMETA(DisplayName = "Furthest"),

	/** Target random. */
	CTM_Random				UMETA(DisplayName = "Random")
};

/**
 *	Base implementation of ChooseTarget task with actor specialization.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetActorTask : public USP_ChooseTargetTask
{
	GENERATED_BODY()
	
protected:
	/** The method of selection of Target from list. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target|Actor")
	ESP_ChooseTargetActorMethod Method = ESP_ChooseTargetActorMethod::CTM_Nearest;

	/** Predicate to validate to choose an Actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Action|Task|Target|Actor")
	bool Predicate(const USP_AIPlannerComponent* Planner, const AActor* Actor) const;

	/** Choose target from list using Method. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Action|Task|Target|Actor")
	AActor* Choose(const USP_AIPlannerComponent* Planner, const TArray<AActor*>& Actors);

	bool PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const override;
	bool ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const override;
};