// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ATASK_GUARD
#define SPLANNER_GOAP_ATASK_GUARD

#include <memory>

#include <SP/GOAP/Base/Action/AAction.hpp>

#include <SP/GOAP/AI/Task/TaskState.hpp>

namespace SP
{
	namespace AI
	{
		class ATask : public AAction
		{
		public:
			virtual void* InstantiateData() const;
			virtual void DeleteData(void* _data) const;

			virtual bool Begin(void* _data) const;
			virtual bool End(TaskState _state, void* _data) const;

			virtual TaskState Tick(float _deltaTime, void* _data) const = 0;
		};

		using TaskPtr = std::shared_ptr<ATask>;
	}
}

#endif // GUARD
