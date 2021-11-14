// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/SP_Task.h>

#include <SP/Collections/Debug>

USP_Task::USP_Task(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskDataClass = USP_TaskData::StaticClass();
}


bool USP_Task::Begin(SP::AI::TaskData* _data) const
{
	SP_RCHECK(_data, "Task data nullptr", Error, false);

	return Begin(static_cast<USP_TaskData*>(_data));
}

bool USP_Task::Begin_Implementation(USP_TaskData* Data) const
{
	(void)Data;

	return true;
}


bool USP_Task::End(SP::AI::TaskState State, SP::AI::TaskData* Data) const
{
	SP_RCHECK(Data, "Task data nullptr", Error, false);

	return End(static_cast<ESP_TaskState>(State), static_cast<USP_TaskData*>(Data));
}

bool USP_Task::End_Implementation(ESP_TaskState State, USP_TaskData* Data) const
{
	(void)State;
	(void)Data;

	return true;
}


SP::AI::TaskState USP_Task::Tick(float DeltaTime, SP::AI::TaskData* Data) const
{
	SP_RCHECK(Data, "Task data nullptr", Error, SP::AI::TaskState::Failure);

	return static_cast<SP::AI::TaskState>(Tick(DeltaTime, static_cast<USP_TaskData*>(Data)));
}

ESP_TaskState USP_Task::Tick_Implementation(float DeltaTime, USP_TaskData* Data) const
{
	return ESP_TaskState::TS_Success;
}


SP::AI::TaskData* USP_Task::InstantiateData() const
{
	return NewObject<USP_TaskData>(GetTransientPackage(), TaskDataClass);
}

void USP_Task::DeleteData(SP::AI::TaskData* _data) const
{
	SP_RCHECK(_data, "Task data nullptr", Error, SP::AI::TaskState::Failure);

	USP_TaskData* uTask = static_cast<USP_TaskData*>(_data);

	uTask->ConditionalBeginDestroy();
}
