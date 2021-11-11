// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AGOAP_PLANNER_GUARD
#define SPLANNER_GOAP_AGOAP_PLANNER_GUARD

#include <vector>

#include <SPlanner/Abstract/APlanner.hpp>
#include <SPlanner/GOAP/Base/Action/ActionHandle.hpp>

namespace SP
{
	class AGOAPPlanner : public APlanner
	{
	public:
		struct GenInfos
		{
			/**
			*	Max planner algorithm depth.
			*	Set to -1 for maximum depth.
			*/
			int maxDepth = -1;

			void* userData = nullptr;

			std::vector<ActionHandle*> shuffledActions;
		};

		virtual std::vector<AAction*> Generate(const GenInfos& _infos) = 0;
	};
}

#endif // GUARD
