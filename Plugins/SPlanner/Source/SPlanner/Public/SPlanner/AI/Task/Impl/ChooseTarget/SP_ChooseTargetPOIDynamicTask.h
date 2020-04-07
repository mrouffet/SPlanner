// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetPOITask.h>
#include "SP_ChooseTargetPOIDynamicTask.generated.h"

/**
 *	Choose target implementation with dynamic POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_ChooseTargetPOIDynamicTask : public USP_ChooseTargetPOITask
{
	GENERATED_BODY()
	
protected:
	/**
	*	List of POI actor type to target.
	*	Set empty to allow every types.
	*	Set bIgnorePOIActorType to true to use this types as ignored types.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target|POI|Dynamic")
	TArray<TSubclassOf<AActor>> POIActorTypes;

	/** Use POIActorTypes as ignored type instead of allowed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target|POI|Dynamic")
	bool bIgnorePOIActorType = false;

	/** Predicate to validate to choose a POI. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Target")
	bool PredicatePOI(const USP_AIPlannerComponent* Planner, const USP_POIComponent* POI) const;

	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_ChooseTargetPOIDynamicTask(const FObjectInitializer& ObjectInitializer);
};