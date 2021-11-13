// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/CompoundTask.hpp>

#include <Collections/Debug>

namespace SP
{
	namespace AI
	{
		void* CompoundTask::InstantiateData() const
		{
			return new CompoundTaskData();
		}

		void CompoundTask::DeleteData(void* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, );

			delete static_cast<CompoundTaskData*>(_data);
		}


		bool CompoundTask::Begin(void* _data) const
		{
			if (tasks.empty())
			{
				SP_LOG("Empty Compound Task", Warning);
				return false;
			}

			SP_RCHECK(_data, "Nullptr task data.", Error, false);
			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			// Begin first task.

			SP_RCHECK(tasks.front(), "Nullptr task registered", Error, false);
			ATask& subTask = *tasks.front();

			taskData.subData = subTask.InstantiateData();
			return subTask.Begin(taskData.subData);
		}

		bool CompoundTask::End(TaskState _state, void* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, false);

			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			// End last task.

			SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, false);
			ATask& subTask = *tasks[taskData.index];

			const bool bSubRes = subTask.End(taskData.subState, taskData.subData);

			subTask.DeleteData(taskData.subData);

			return bSubRes;
		}


		TaskState CompoundTask::Tick(float _deltaTime, void* _data) const
		{
			SP_RCHECK(_data, "Nullptr task data.", Error, TaskState::Failure);
			auto& taskData = *static_cast<CompoundTaskData*>(_data);

			SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, TaskState::Failure);
			ATask& subTask = *tasks[taskData.index];

			taskData.subState = subTask.Tick(_deltaTime, taskData.subData);

			if (taskData.subState != TaskState::Success)
				return taskData.subState; // Failed or running.


			// Last task's tick succeed: call sub task's End() on this.End().
			if (taskData.index + 1 == tasks.size())
				return TaskState::Success;


			// End sub task.
			if (!subTask.End(taskData.subState, taskData.subData))
				return TaskState::Failure;

			subTask.DeleteData(taskData.subData);


			// Begin next task.
			++taskData.index;

			SP_RCHECK(tasks[taskData.index], "Nullptr task registered", Error, TaskState::Failure);
			ATask& nextSubTask = *tasks[taskData.index];

			taskData.subData = nextSubTask.InstantiateData();

			if (!nextSubTask.Begin(taskData.subData))
				return TaskState::Failure;

			return TaskState::Pending;
		}
	}
}
