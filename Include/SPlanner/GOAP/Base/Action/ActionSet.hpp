// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACTION_SET_GUARD
#define SPLANNER_GOAP_ACTION_SET_GUARD

#include <vector>

#include <SPlanner/GOAP/Base/Action/ActionHandle.hpp>

namespace SP
{
	class ActionSet
	{
	public:
		std::vector<ActionHandle> actionHandles;
	};
}

#endif // GUARD
