// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/CompoundTask.hpp>

#include <Collections/Debug>

namespace SP
{
	namespace AI
	{
		TaskData* CompoundTask::InstantiateData() const
		{
			return new CompoundTaskData{};
		}

		void CompoundTask::DeleteData(TaskData* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error,);

			delete static_cast<CompoundTaskData*>(_data);
		}


		bool CompoundTask::Begin(TaskData* _data) const
		{
			if (tasks.empty())
			{
				SP_LOG("Empty Compound Task", Warning);
				return false;
			}

			SP_RCHECK(_data, "Nullptr task data.", Error, false);
			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			// Instantiate first task data.

			SP_RCHECK(tasks.front(), "Nullptr task registered", Error, false);
			ATask& subTask = *tasks.front();

			taskData.subData = subTask.InstantiateData();

			return true;
		}

		bool CompoundTask::End(TaskState _state, TaskData* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, false);
			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			if (_state == TaskState::ForceEnd)
			{
				SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, false);
				ATask& subTask = *tasks[taskData.index];

				subTask.DeleteData(taskData.subData);
			}
			
			return true;
		}


		TaskState CompoundTask::Tick(float _deltaTime, TaskData* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, TaskState::Failure);
			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, TaskState::Failure);
			ATask& subTask = *tasks[taskData.index];

			const TaskState subState = subTask.Update(_deltaTime, taskData.subData);

			if (subState == TaskState::Pending)
				return TaskState::Pending;

			// Clean data.
			subTask.DeleteData(taskData.subData);

			if (subState == TaskState::Success)
			{
				// Last task succeeded.
				if (++taskData.index == tasks.size())
					return TaskState::Success;

				// Prepare next task.
				SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, TaskState::Failure);
				ATask& nextSubTask = *tasks[taskData.index];

				taskData.subData = nextSubTask.InstantiateData();

				return TaskState::Pending;
			}
			else if (subState & TaskState::Failure)
				return TaskState::Failure;
		}
	}
}
