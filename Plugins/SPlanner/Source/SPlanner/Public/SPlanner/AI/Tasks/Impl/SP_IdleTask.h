// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_IdleTask.generated.h"

/**
 *	Idle task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_IdleTask : public USP_Task
{
	GENERATED_BODY()
	

protected:
	struct FSP_IdleTaskInfos : public FSP_TaskInfos
	{
		float IT_WaitTime = -1.0f;
		float IT_CurrTime = 0.0f;
	};

	/** The minimum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MinTime = 0.5f;

	/** The maximum idle time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	float MaxTime = 3.0f;

	void ConstructUserData(uint8* UserData) override;
	void DestructUserData(uint8* UserData) override;

public:
	uint32 GetUserDataSize() const override;

	bool Begin(USP_AIPlannerComponent& Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData) override;
};