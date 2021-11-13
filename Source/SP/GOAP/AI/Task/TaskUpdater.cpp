// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/TaskUpdater.hpp>

#include <Collections/Debug>

namespace SP
{
	namespace AI
	{
		void TaskUpdater::SetTask(TaskPtr _task)
		{
			// Task started but not finished: automatic failure.
			if (bHasBegun)
			{
				SP_RCHECK(mHandle, "Handled Task nullptr", Error,);

				mHandle->End(TaskState::ForceEnd, mTaskData);
				mHandle->DeleteData(mTaskData);

				bHasBegun = false;
			}

			mHandle = _task;
		}

		TaskState TaskUpdater::Update(float _deltaTime)
		{
			SP_RCHECK(mHandle, "Handled Task nullptr", Error, TaskState::Failure);

			if (!bHasBegun)
			{
				mTaskData = mHandle->InstantiateData();
				mHandle->Begin(mTaskData);

				bHasBegun = true;
			}

			const TaskState state = mHandle->Tick(_deltaTime, mTaskData);

			if (state == TaskState::Pending)
				return TaskState::Pending;

			mHandle->End(state, mTaskData);
			mHandle->DeleteData(mTaskData);

			bHasBegun = false;

			return state;
		}
	}
}
