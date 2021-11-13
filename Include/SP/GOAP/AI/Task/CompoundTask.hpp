// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_COMPOND_TASK_GUARD
#define SPLANNER_GOAP_COMPOND_TASK_GUARD

#include <vector>

#include <SP/GOAP/AI/Task/ATask.hpp>

namespace SP
{
	namespace AI
	{
		class CompoundTask : public ATask
		{
		public:
			std::vector<TaskPtr> tasks;

			void* InstantiateData() const override final;
			void DeleteData(void* _data) const override final;

			bool Begin(void* _data) const override final;
			bool End(TaskState _state, void* _data) const override final;

			TaskState Tick(float _deltaTime, void* _data) const override final;
		};

		struct CompoundTaskData
		{
			size_t index = 0;

			TaskState subState = TaskState::Success;

			void* subData = nullptr;
		};
	}
}

#endif // GUARD
