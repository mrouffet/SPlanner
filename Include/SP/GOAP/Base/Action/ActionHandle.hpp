// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACTION_HANDLE_GUARD
#define SPLANNER_GOAP_ACTION_HANDLE_GUARD

#include <vector>

#include <SP/GOAP/Base/Action/AAction.hpp>
#include <SP/GOAP/Base/Action/Weight/AWeight.hpp>
#include <SP/GOAP/Base/Action/Conditions/APreCondition.hpp>
#include <SP/GOAP/Base/Action/Conditions/APostCondition.hpp>

/**
*	STRIPS based architecture: https://en.wikipedia.org/wiki/Stanford_Research_Institute_Problem_Solver
*	F.E.A.R inspiration: https://en.wikipedia.org/wiki/F.E.A.R._(video_game)
*/

namespace SP
{
	class ActionHandle
	{
	public:
		AAction* action = nullptr;
		AWeight* weight = nullptr;

		std::vector<const APreCondition*> preconditions;
		std::vector<const APostCondition*> postconditions;

		bool bAchievePlan = false;

		~ActionHandle();

		bool ValidatePreConditions(void* _userData) const;
		bool ValidatePostConditions(void* _userData) const;
	};
}

#endif // GUARD
