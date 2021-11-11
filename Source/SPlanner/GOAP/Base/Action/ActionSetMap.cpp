// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/ActionSetMap.hpp>

#include <Collections/Debug>

namespace SP
{
	const ActionSet* ActionSetMap::Get(const AGoal* _goal)
	{
		SP_RCHECK(_goal, "Get nullptr goal!", Error, nullptr);

		auto it = mMap.find(_goal);

		return it != mMap.end() ? it->second : nullptr;
	}

	
	void ActionSetMap::Emplace(const AGoal* _goal, const ActionSet* _set)
	{
		SP_RCHECK(_goal, "Emplace nullptr goal!", Error,);

		mMap[_goal] = _set;
	}
	
	void ActionSetMap::Erase(const AGoal* _goal)
	{
		SP_RCHECK(_goal, "Erase nullptr goal!", Error,);

		mMap.erase(_goal);
	}

	
	void ActionSetMap::Clear()
	{
		mMap.clear();
	}
}
