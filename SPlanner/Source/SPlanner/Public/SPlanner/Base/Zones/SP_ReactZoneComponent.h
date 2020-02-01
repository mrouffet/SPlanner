// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Zones/SP_ZoneComponent.h>
#include "SP_ReactZoneComponent.generated.h"

class USP_Goal;

class USP_PlannerComponent;

USTRUCT(BlueprintType, Category = "SPlanner|ReactZone")
struct FSP_ReactZonePair
{
	GENERATED_BODY()

	/** The goal to set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* Goal = nullptr;
	
	/** The number of overlapped object required. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	int ObjectNum = 1;
};

/**
 *	React zone implementation: switch goal on enter / exit.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "SP_ReactZone", ClassGroup = "SPlanner|Zone|React", meta=(BlueprintSpawnableComponent))
class SPLANNER_API USP_ReactZoneComponent : public USP_ZoneComponent
{
	GENERATED_BODY()
	
protected:
	/** Goal lists depending on number of overlapping objects. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_ReactZonePair> GoalPairs;

	/** Should save previous goal as ExitGoal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bSavePreviousGoal = false;

	/** The new goal to set when object with ReactObjectTag enter overlap.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner", meta=(EditCondition="!bSavePreviousGoal"))
	USP_Goal* ExitGoal = nullptr;

	/** Cached planner component. */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	/** React from add registered objects. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Zone|React")
	void ReactOnAdd();

	/** React from add registered objects. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner|Zone|React")
	void ReactOnRemove();

	void AddObject_Implementation(UPrimitiveComponent* Object) override;
	void RemoveObject_Implementation(UPrimitiveComponent* Object) override;

public:
	USP_ReactZoneComponent(const FObjectInitializer& ObjectInitializer);

	/** Set the Planner. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Zone|React")
	void SetPlanner(USP_PlannerComponent* InPlanner);
};
