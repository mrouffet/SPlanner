// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CONFIG_GUARD
#define SPLANNER_CONFIG_GUARD

/**
*	\file Config.hpp
* 
*	\brief SPlanner config file.
*/


/// SPlanner global namespace
namespace SP
{
	/// SPlanner Documentation classes implementations.
	namespace Doc
	{
	}

	/// \cond Internal

	/// SPlanner Internal classes implementations.
	namespace Intl
	{
	}

	/// \endcond Internal
}


/**
*	\defgroup Debug Debug
*	SPlanner Debug tools.
*/


/**
*	\defgroup Misc Miscellaneous
*	SPlanner Miscellaneous tools.
*/


/**
*	\defgroup GOAP Goal Oriented Action Planning
*	SPlanner GOAP (Goal Oriented Action Planning) tools.
*/

/**
*	\defgroup AI AI
*	SPlanner AI specific tools.
*	\ingroup GOAP
*/

#endif // GUARD
