// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <GOAP/AI/Task/SP_Task.h>
#include "SP_IdleTask.generated.h"

/**
 *
 */
UCLASS(ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_IdleTask : public USP_Task
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

	bool Begin_Implementation(USP_TaskData* Data) const override;
	ESP_TaskState Tick_Implementation(float DeltaTime, USP_TaskData* Data) const override;

public:
	USP_IdleTask(const FObjectInitializer& ObjectInitializer);
};


UCLASS(ClassGroup = "SPlanner|Task")
class USP_IdleTaskData : public USP_TaskData
{
	GENERATED_BODY()

public:
	/// Generated time to idle.
	float WaitTime = -1.0f;

	/// Current idle tick time.
	float CurrTime = -1.0f;
};
