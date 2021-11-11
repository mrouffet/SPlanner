// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CONFIG_GUARD
#define SPLANNER_CONFIG_GUARD

/**
*	\file Config.hpp
* 
*	\brief SPlanner config file.
*/


#if !defined(NDEBUG) || defined(_DEBUG)

	/// SPlanner Debug preprocessor.
	#define SP_DEBUG 1

#else

	/// SPlanner Debug preprocessor.
	#define SP_DEBUG 0

#endif


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

#endif // GUARD
