// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/Task/ATask.hpp>

#include <Collections/Debug>

namespace SP
{
	namespace AI
	{
		void* ATask::InstantiateData() const
		{
			return nullptr;
		}
		
		void ATask::DeleteData(void* _data) const
		{
			(void)_data;
		}


		bool ATask::Begin(void* _data) const
		{
			(void)_data;

			return true;
		}

		bool ATask::End(TaskState _state, void* _data) const
		{
			(void)_state;
			(void)_data;

			return true;
		}
	}
}
