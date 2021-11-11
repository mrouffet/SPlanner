// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ATASK_GUARD
#define SPLANNER_GOAP_ATASK_GUARD

#include <SPlanner/GOAP/AI/Task/TaskState.hpp>

namespace SP
{
	class ATask
	{
	public:
		virtual bool Begin();
		virtual bool End();

		virtual TaskState Tick(float _deltaTime) = 0;
	};
}

#endif // GUARD
