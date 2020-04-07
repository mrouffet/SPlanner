// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleBaseTask.h>
#include "SP_IdleTask.generated.h"

/**
 *	Idle task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_IdleTask : public USP_IdleBaseTask
{
	GENERATED_BODY()
	
protected:
	/**
	* The minimum idle time.
	* Set < 0.0f to not use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MinTime = 0.5f;

	/**
	* The maximum idle time.
	* Set < 0.0f to not use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MaxTime = 3.0f;

	float GenerateTime_Implementation(const USP_AIPlannerComponent* Planner) override;
};