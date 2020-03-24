// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Zone/SP_ZoneComponent.h>
#include "SP_ToogleActiveZoneComponent.generated.h"

class USP_PlannerComponent;

/**
 *	Toogle zone implementation: Set active/inactive handled Planners.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "SP_ToogleActiveZone", ClassGroup = "SPlanner|Zone", meta = (BlueprintSpawnableComponent))
class SPLANNER_API USP_ToogleActiveZoneComponent : public USP_ZoneComponent
{
	GENERATED_BODY()
	
protected:
	/** The default active state (ie: no colliding object). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bDefaultActiveState = false;

	/** Handled planners. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_PlannerComponent*> Planners;

	void AddObject_Implementation(UPrimitiveComponent* Object) override;
	void RemoveObject_Implementation(UPrimitiveComponent* Object) override;

public:
	/** Add a planner to handle in Planners.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Zone|Toogle")
	void Add(USP_PlannerComponent* InPlanner);

	/** Add planners to handle in Planners.*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Zone|Toogle")
	void AddRange(const TArray<USP_PlannerComponent*>& InPlanners);

	/** Remove a planner to handle in Planners.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Zone|Toogle")
	void Remove(USP_PlannerComponent* InPlanner);

	/** Remove planners to handle in Planners.*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Zone|Toogle")
	void RemoveRange(const TArray<USP_PlannerComponent*>& InPlanners);
};