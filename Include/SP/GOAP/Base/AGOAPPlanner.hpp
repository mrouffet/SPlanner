// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AGOAP_PLANNER_GUARD
#define SPLANNER_GOAP_AGOAP_PLANNER_GUARD

#include <vector>

#include <SP/Abstract/APlanner.hpp>
#include <SP/GOAP/Base/Action/ActionHandle.hpp>

/**
*	\file AGOAPPlanner.hpp
* 
*	\brief GOAP planner base type definition.
* 
*	Sources:
*	https://en.wikipedia.org/wiki/Action_selection
*	https://alumni.media.mit.edu/~jorkin/goap.html#:~:text=Goal%2DOriented%20Action%20Planning%20(aka,autonomous%20character%20behavior%20in%20games.&text=architecture%20simultaneously%20powered%20Monolith's%20Condemned%3A%20Criminal%20Origins.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	\brief GOAP (Goal Oriented Action Planning) base class.
	*/
	class AGOAPPlanner : public APlanner
	{
	public:
		struct GenInfos
		{
			/**
			*	Max planner algorithm depth.
			*	Set to -1 for maximum depth.
			*/
			uint32_t maxDepth = uint32_t(-1);

			void* userData = nullptr;

			std::vector<ActionHandle*> shuffledActions;
		};

		virtual std::vector<const AAction*> Generate(const GenInfos& _infos) = 0;
	};
}

/** \} */

#endif // GUARD
