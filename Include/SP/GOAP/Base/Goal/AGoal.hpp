// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AGOAL_GUARD
#define SPLANNER_GOAP_AGOAL_GUARD

#include <memory>

/**
*	\file AGoal.hpp
* 
*	\brief Goal base type definition.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	\brief Goal base type.
	*/
	class AGoal
	{
	};


	/// Alias for task ptr (should always be shared).
	using GoalPtr = std::shared_ptr<AGoal>;
}

/** \} */

#endif // GUARD
