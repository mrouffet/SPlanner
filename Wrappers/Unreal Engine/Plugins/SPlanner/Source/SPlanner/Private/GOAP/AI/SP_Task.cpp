// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/SP_Task.h>

#include <SP/Collections/Debug>

bool USP_Task::Begin(SP::AI::TaskData* _data) const
{
	SP_RCHECK(_data, "Task data nullptr", Error, false);

	return Begin(static_cast<USP_TaskData*>(_data));
}

bool USP_Task::Begin_Implementation(USP_TaskData* _data) const
{
	(void)_data;

	return true;
}


bool USP_Task::End(SP::AI::TaskState _state, SP::AI::TaskData* _data) const
{
	SP_RCHECK(_data, "Task data nullptr", Error, false);

	return End(static_cast<ESP_TaskState>(_state), static_cast<USP_TaskData*>(_data));
}

bool USP_Task::End_Implementation(ESP_TaskState _state, USP_TaskData* _data) const
{
	(void)_state;
	(void)_data;

	return true;
}


SP::AI::TaskState USP_Task::Tick(float _deltaTime, SP::AI::TaskData* _data) const
{
	SP_RCHECK(_data, "Task data nullptr", Error, SP::AI::TaskState::Failure);

	return static_cast<SP::AI::TaskState>(Tick(_deltaTime, static_cast<USP_TaskData*>(_data)));
}

ESP_TaskState USP_Task::Tick_Implementation(float _deltaTime, USP_TaskData* _data) const
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
