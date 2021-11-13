// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_LOG_LEVEL_GUARD
#define SPLANNER_CORE_LOG_LEVEL_GUARD

/**
*	\file LogLevel.hpp
* 
*	\brief Log level type definition.
* 
*	\ingroup Debug
*	\{
*/

namespace SP
{
	/**
	*	SPlanner Log level enum type.
	*/
	enum class LogLevel
	{
		/// Default level.
		Normal,

		/// Infos level.
		Infos,

		/// Warning level.
		Warning,

		/// Error level.
		Error
	};
}

/** \} */

#endif // GUARD
