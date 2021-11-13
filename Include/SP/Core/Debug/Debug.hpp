// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_DEBUG_GUARD
#define SPLANNER_CORE_DEBUG_GUARD

#include <SP/Core/Debug/Log.hpp>
#include <SP/Core/Debug/FileName.hpp>
#include <SP/Core/Debug/FuncName.hpp>

/**
*	\file Debug.hpp
* 
*	\brief SPlanner Debug macros definition.
* 
*	\ingroup Debug
*	\{
*/

namespace SP
{
#if !defined(NDEBUG) || defined(_DEBUG) || defined(DOXYGEN)

	/// SPlanner Debug preprocessor.
	#define SP_DEBUG 1

#else

	#define SP_DEBUG 0

#endif

	/// API log callback.
	static void (*logCallback)(const Log& _log) = nullptr;

#if SP_DEBUG || defined(DDOXYGEN)

	/// \cond Internal

	#define __SP_MAKE_LOG(_str, _level) Log{\
			SP::LogLevel::_level,\
			__SP_FILE_NAME,\
			__LINE__,\
			__SP_FUNC_NAME,\
			_str\
		}

	/// \endcond

	/**
	*	\def SP_LOG(_str, _level)
	* 
	*	\brief SPlanner Log macro.
	* 
	*	Helper macro to use log callback.
	* 
	*	\param[in] _str		String message of the log.
	*	\param[in] _level	Level of the log.
	*/
	#define SP_LOG(_str, _level) if(logCallback) { logCallback(__SP_MAKE_LOG(_str, _level)); }

	/**
	*	\def SP_CHECK(_pred, _str, _level)
	* 
	*	\brief Log on !predicate.
	*	Predicate should be verified (otherwise log).
	* 
	*	\param[in] _pred	Predicate to verify.
	*	\param[in] _str		String message of the log.
	*	\param[in] _level	Level of the log.
	*/
	#define SP_CHECK(_pred, _str, _level) if(!(_pred)) { SP_LOG(_str, _level) }

	/**
	*	\def SP_RCHECK(_pred, _str, _level, _return)
	* 
	*	\brief Log on !predicate and return function.
	*	Predicate should be verified (otherwise log).
	* 
	*	\param[in] _pred	Predicate to verify.
	*	\param[in] _str		String message of the log.
	*	\param[in] _level	Level of the log.
	*	\param[in] _return	Value to return on failure.
	*/
	#define SP_RCHECK(_pred, _str, _level, _return) if(!(_pred)) { SP_LOG(_str, _level) return _return; }

#else

	#define SP_LOG(...) {}

	#define SP_CHECK(...) {}
	#define SP_RCHECK(...) {}

#endif
}


/** \} */

#endif // GUARD
