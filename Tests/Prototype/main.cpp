// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <iostream>

#define LOG(_str) std::cout << _str << std::endl;

#include <SP/Collections/Debug>
#include <SP/GOAP/Base/Action/ActionSetMap.hpp>
using namespace SP;

AGoal patrolGoal;
ActionSet patrolSet;

AGoal idleGoal;
ActionSet idleSet;

ActionSetMap actionSetMap;

void LogCallback(const SP::Log& _log);


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
	SP::logCallback = LogCallback;

	Init();

	UnInit();

#if !SA_CI

	std::cin.get();

#endif

	return 0;
}

void LogCallback(const SP::Log& _log)
{
	std::string level;

	switch (_log.level)
	{
		case LogLevel::Normal:
			level = "[Normal]";
			break;
		case LogLevel::Infos:
			level = "[Infos]";
			break;
		case LogLevel::Warning:
			level = "[Warning]";
			break;
		case LogLevel::Error:
			level = "[Error]";
			break;
		default:
			level = "[Unknown]";
			break;
	}

	LOG(level << " in " << _log.function << " at " << _log.file << ":" << _log.line << ":\n" << _log.message);
}
