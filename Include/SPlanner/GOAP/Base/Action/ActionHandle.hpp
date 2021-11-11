// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACTION_HANDLE_GUARD
#define SPLANNER_GOAP_ACTION_HANDLE_GUARD

#include <memory>
#include <vector>

#include <SPlanner/GOAP/Base/Action/AAction.hpp>
#include <SPlanner/GOAP/Base/Action/Conditions/APreCondition.hpp>
#include <SPlanner/GOAP/Base/Action/Conditions/APostCondition.hpp>
#include <SPlanner/GOAP/Base/Action/Weights/AWeight.hpp>

/**
*	STRIPS based architecture: https://en.wikipedia.org/wiki/Stanford_Research_Institute_Problem_Solver
*	F.E.A.R inspiration: https://en.wikipedia.org/wiki/F.E.A.R._(video_game)
*/

namespace SP
{
	class ActionHandle
	{
	public:
		std::shared_ptr<const AAction> action = nullptr;
		std::shared_ptr<const AWeight> weight = nullptr;

		std::vector<const APreCondition*> preconditions;
		std::vector<const APostCondition*> postconditions;

		bool bAchievePlan = false;

		~ActionHandle();

		bool ValidatePreCondition(void* _userData) const;
		bool ValidatePostCondition(void* _userData) const;
	};
}

#endif // GUARD
