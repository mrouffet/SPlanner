// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/SP_IdleTask.h>

#include <SP/Collections/Debug>

USP_IdleTask::USP_IdleTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskDataClass = USP_IdleTaskData::StaticClass();
}


bool USP_IdleTask::Begin_Implementation(USP_TaskData* Data) const
{
	if (!Super::Begin_Implementation(Data))
		return false;

	USP_IdleTaskData* const IdleData = Cast<USP_IdleTaskData>(Data);
	SP_RCHECK(IdleData, "Data nullptr! TaskData must be of type USP_IdleTaskData", Error, false);

	IdleData->WaitTime = FMath::RandRange(MinTime, MaxTime);

	return true;
}

ESP_TaskState USP_IdleTask::Tick_Implementation(float DeltaTime, USP_TaskData* Data) const
{
	const ESP_TaskState SuperState = Super::Tick_Implementation(DeltaTime, Data);

	if (SuperState != ESP_TaskState::TS_Success)
		return SuperState;

	USP_IdleTaskData* const IdleData = Cast<USP_IdleTaskData>(Data);
	SP_RCHECK(IdleData, "Data nullptr! TaskData must be of type USP_IdleTaskData", Error, ESP_TaskState::TS_Failure);

	IdleData->CurrTime += DeltaTime;

	if (IdleData->CurrTime >= IdleData->WaitTime)
		return ESP_TaskState::TS_Success;

	return ESP_TaskState::TS_Pending;
}
