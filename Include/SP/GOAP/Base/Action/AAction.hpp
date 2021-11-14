// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AACTION_GUARD
#define SPLANNER_GOAP_AACTION_GUARD

#include <SP/Abstract/AStep.hpp>

/**
*	\file AAction.hpp
* 
*	\brief GOAP Action base type.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	GOAP planner's action base type.
	*/
	class AAction : public AStep
	{
	public:
		virtual ~AAction() = default;
	};
}

/** \} */

#endif // GUARD
