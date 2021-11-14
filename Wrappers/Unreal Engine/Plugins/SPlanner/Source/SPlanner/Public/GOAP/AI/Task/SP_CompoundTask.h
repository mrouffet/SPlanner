// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <GOAP/AI/Task/SP_Task.h>

#include <SP/GOAP/AI/Task/CompoundTask.hpp>

#include "SP_CompoundTask.generated.h"

/**
 *
 */
UCLASS(ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_CompoundTask : public USP_Task
{
	GENERATED_BODY()

protected:
	/// Wrapped compound task.
	SP::AI::CompoundTask mHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Compound")
	TArray<USP_Task*> Tasks;


	void InitHandle();

	bool Begin_Implementation(USP_TaskData* Data) const override;
	bool End_Implementation(ESP_TaskState State, USP_TaskData* Data) const override;
	ESP_TaskState Tick_Implementation(float DeltaTime, USP_TaskData* Data) const override;

public:
	USP_CompoundTask(const FObjectInitializer& ObjectInitializer);

	void PostLoad() override;

#if WITH_EDITOR

	bool Modify(bool bAlwaysMarkDirty) override;

#endif
};

UCLASS(ClassGroup = "SPlanner|Task")
class USP_CompoundTaskData : public USP_TaskData
{
	GENERATED_BODY()

public:
	SP::AI::CompoundTaskData handle;
};
