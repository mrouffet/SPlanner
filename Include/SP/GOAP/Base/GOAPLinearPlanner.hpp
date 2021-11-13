// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_GOAP_LINEAR_PLANNER_GUARD
#define SPLANNER_GOAP_GOAP_LINEAR_PLANNER_GUARD

#include <SP/GOAP/Base/AGOAPPlanner.hpp>

namespace SP
{
	class GOAPLinearPlanner : public AGOAPPlanner
	{
		bool Generate_Internal(const GenInfos& _infos, std::vector<const AAction*>& _outPlan, uint32_t _actionIndex = 0u, uint32_t _currDepth = 0u);

	public:
		std::vector<const AAction*> Generate(const GenInfos& _infos) override final;
	};
}

#endif // GUARD