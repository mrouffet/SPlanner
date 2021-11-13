// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACTION_SET_MAP_GUARD
#define SPLANNER_GOAP_ACTION_SET_MAP_GUARD

#include <unordered_map>

#include <SP/GOAP/Base/Goal/AGoal.hpp>
#include <SP/GOAP/Base/Action/ActionSet.hpp>

namespace SP
{
	class ActionSetMap
	{
		std::unordered_map<const AGoal*, const ActionSet*> mMap;

	public:
		const ActionSet* Get(const AGoal* _goal);

		void Emplace(const AGoal* _goal, const ActionSet* _set);
		void Erase(const AGoal* _goal);
		
		void Clear();
	};
}

#endif // GUARD
