// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACTION_SET_GUARD
#define SPLANNER_GOAP_ACTION_SET_GUARD

#include <vector>

#include <SP/GOAP/Base/Action/ActionHandle.hpp>

namespace SP
{
	class AWeightDistribution;

	class ActionSet
	{
	public:
		std::vector<ActionHandle> actionHandles;

		std::vector<const ActionHandle*> Shuffle(AWeightDistribution* _dist) const;
	};
}

#endif // GUARD
