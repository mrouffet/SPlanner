// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_CORE_DEBUG_GUARD
#define SPLANNER_CORE_DEBUG_GUARD

#include <SP/Core/Debug/Log.hpp>
#include <SP/Core/Debug/FileName.hpp>
#include <SP/Core/Debug/FuncName.hpp>

namespace SP
{
#if !defined(NDEBUG) || defined(_DEBUG)

	/// SPlanner Debug preprocessor.
	#define SP_DEBUG 1

#else

	/// SPlanner Debug preprocessor.
	#define SP_DEBUG 0

#endif

	inline void (*logCallback)(const Log& _log) = nullptr;

#if SP_DEBUG

#define __SP_MAKE_LOG(_str, _level) Log{\
		SP::LogLevel::_level,\
		__SP_FILE_NAME,\
		__LINE__,\
		__SP_FUNC_NAME,\
		_str\
	}

#define SP_LOG(_str, _level) if(logCallback) { logCallback(__SP_MAKE_LOG(_str, _level)); }

#define SP_CHECK(_pred, _str, _level) if(!(_pred)) { SP_LOG(_str, _level) }
#define SP_RCHECK(_pred, _str, _level, _return) if(!(_pred)) { SP_LOG(_str, _level) return _return; }

#else

	#define SP_LOG(...) {}

	#define SP_CHECK(...) {}
	#define SP_RCHECK(...) {}

#endif
}

#endif // GUARD
