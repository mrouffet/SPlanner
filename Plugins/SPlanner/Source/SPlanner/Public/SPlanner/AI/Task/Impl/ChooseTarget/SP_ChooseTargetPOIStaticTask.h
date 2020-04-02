// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetPOITask.h>
#include "SP_ChooseTargetPOIStaticTask.generated.h"

/**
 *	Choose target implementation with static POI spec.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_ChooseTargetPOIStaticTask : public USP_ChooseTargetPOITask
{
	GENERATED_BODY()
	
protected:
	/** Static List of all POI. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Task|Target|POI|Static")
	TArray<USP_POIComponent*> POIs;

	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	/** Register a POI into POIs*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target|POI")
	void Register(USP_POIComponent* POI);

	/** UnRegister a POI into POIs*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target|POI")
	void UnRegister(USP_POIComponent* POI);
};