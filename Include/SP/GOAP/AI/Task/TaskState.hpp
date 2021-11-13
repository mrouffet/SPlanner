// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_TASK_STATE_GUARD
#define SPLANNER_GOAP_TASK_STATE_GUARD

namespace SP
{
	namespace AI
	{
		enum class TaskState
		{
			Success = 0,

			Failure = 1,

			Pending = 2,
		};
	}
}

#endif // GUARD
