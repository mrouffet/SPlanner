// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/ActionSetMap.hpp>

namespace SP
{
	const ActionSet* ActionSetMap::Get(const AGoal* _goal)
	{
		// TODO: Check nullptr.

		auto it = mMap.find(_goal);

		return it != mMap.end() ? it->second : nullptr;
	}

	
	void ActionSetMap::Emplace(const AGoal* _goal, const ActionSet* _set)
	{
		// TODO: Check nullptr.

		mMap[_goal] = _set;
	}
	
	void ActionSetMap::Erase(const AGoal* _goal)
	{
		// TODO: Check nullptr.

		mMap.erase(_goal);
	}

	
	void ActionSetMap::Clear()
	{
		mMap.clear();
	}
}
