// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_LOG_GUARD
#define SPLANNER_CORE_LOG_GUARD

#include <string>

#include <SP/Core/Debug/LogLevel.hpp>

/**
*	\file Log.hpp
*
*	\brief SPlanner Log type definition.
*
*	\ingroup Debug
*	\{
*/

namespace SP
{
	/**
	*	SPlanner Log type.
	*/
	class Log
	{
	public:
		/// Output level.
		LogLevel level;

		/// File name.
		std::string file;

		/// Line number.
		uint32_t line = 0u;

		/// Function name.
		std::string function;

		/// User string message.
		std::string message;
	};
}

/** \} */

#endif // GUARD
