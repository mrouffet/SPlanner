// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <iostream>

#define LOG(_str) std::cout << _str << std::endl;

#include <SPlanner/GOAP/Base/Action/ActionSetMap.hpp>
using namespace SP;

AGoal patrolGoal;
ActionSet patrolSet;

AGoal idleGoal;
ActionSet idleSet;

ActionSetMap actionSetMap;

void Init()
{
	actionSetMap.Emplace(&patrolGoal, &patrolSet);
	actionSetMap.Emplace(&idleGoal, &idleSet);
}

void UnInit()
{
	actionSetMap.Clear();
}

int main()
{
	Init();

	UnInit();

#if !SA_CI

	std::cin.get();

#endif

	return 0;
}
