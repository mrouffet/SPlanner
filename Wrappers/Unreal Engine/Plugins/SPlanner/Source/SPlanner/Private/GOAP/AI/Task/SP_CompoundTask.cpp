// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/SP_CompoundTask.h>

#include <SP/Collections/Debug>

USP_CompoundTask::USP_CompoundTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskDataClass = USP_CompoundTaskData::StaticClass();
}


void USP_CompoundTask::InitHandle()
{
	mHandle.tasks.clear();
	mHandle.tasks.reserve(Tasks.Num());

	for (auto uTask : Tasks)
		mHandle.tasks.emplace_back(uTask);
}


bool USP_CompoundTask::Begin_Implementation(USP_TaskData* Data) const
{
	if (!Super::Begin_Implementation(Data))
		return false;

	USP_CompoundTaskData* const CompoundData = Cast<USP_CompoundTaskData>(Data);
	SP_RCHECK(CompoundData, "Data nullptr! TaskData must be of type USP_CompoundTaskData", Error, false);

	return mHandle.Begin(&CompoundData->handle);
}

bool USP_CompoundTask::End_Implementation(ESP_TaskState State, USP_TaskData* Data) const
{
	if (!Super::End_Implementation(State, Data))
		return false;

	USP_CompoundTaskData* const CompoundData = Cast<USP_CompoundTaskData>(Data);
	SP_RCHECK(CompoundData, "Data nullptr! TaskData must be of type USP_CompoundTaskData", Error, false);

	return mHandle.End(static_cast<SP::AI::TaskState>(State), &CompoundData->handle);
}

ESP_TaskState USP_CompoundTask::Tick_Implementation(float DeltaTime, USP_TaskData* Data) const
{
	const ESP_TaskState SuperState = Super::Tick_Implementation(DeltaTime, Data);

	if (SuperState != ESP_TaskState::TS_Success)
		return SuperState;

	USP_CompoundTaskData* const CompoundData = Cast<USP_CompoundTaskData>(Data);
	SP_RCHECK(CompoundData, "Data nullptr! TaskData must be of type USP_CompoundTaskData", Error, ESP_TaskState::TS_Failure);

	return static_cast<ESP_TaskState>(mHandle.Tick(DeltaTime, &CompoundData->handle));
}


void USP_CompoundTask::PostLoad()
{
	Super::PostLoad();

	InitHandle();
}


#if WITH_EDITOR

bool USP_CompoundTask::Modify(bool bAlwaysMarkDirty)
{
	const bool bSuperModify = Super::Modify(bAlwaysMarkDirty);

	InitHandle();

	return bSuperModify;
}

#endif
