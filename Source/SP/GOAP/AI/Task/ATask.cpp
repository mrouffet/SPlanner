// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/ATask.hpp>

#include <Collections/Debug>

namespace SP
{
	namespace AI
	{
		TaskData* ATask::InstantiateData() const
		{
			return new TaskData{};
		}
		
		void ATask::DeleteData(TaskData* _data) const
		{
			delete _data;
		}


		bool ATask::Begin(TaskData* _data) const
		{
			(void)_data;

			return true;
		}

		bool ATask::End(TaskState _state, TaskData* _data) const
		{
			(void)_state;
			(void)_data;

			return true;
		}

		TaskState ATask::Tick(float _deltaTime, TaskData* _data) const
		{
			(void)_deltaTime;
			(void)_data;

			return TaskState::Success;
		}


		TaskState ATask::Update(float _deltaTime, TaskData* _data)
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, TaskState::Failure);

			// Begin Task.
			if (!_data->bHasBegun && Begin(_data))
			{
				// Begin failed.
				
				End(TaskState::BeginFailed, _data);

				return TaskState::BeginFailed;
			}


			// Tick Task.
			const TaskState state = Tick(_deltaTime, _data);

			// Wait for success or failure.
			if (state == TaskState::Pending)
				return TaskState::Pending;


			// End Task.
			if (End(state, _data))
			{
				// End failed.

				return TaskState::EndFailed;
			}

			// Return either tick has succeeded or failed.
			return state;
		}


		void ATask::Terminate(TaskData* _data)
		{
			SP_RCHECK(_data, "Nullptr task data.", Error,);

			if (_data->bHasBegun)
				End(TaskState::ForceEnd, _data);
		}
	}
}
