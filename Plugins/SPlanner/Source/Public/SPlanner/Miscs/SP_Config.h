#pragma once

#include <SPlanner/Debug/SP_DebugMaco.h>

#if !UE_BUILD_SHIPPING

	#define SP_DEBUG 1

	#define SP_DEBUG_EDITOR WITH_EDITOR

	// No Multithread in debug: easy breakpoint debug.
	#define SP_MULTITHREAD 0

	#define SP_WARNING_PLAN_GEN_FAILED 1

#else

	#define SP_DEBUG 0

	#define SP_DEBUG_EDITOR 0

	// TODO: FIX MULTITHREAD RANDOM
	#define SP_MULTITHREAD 0

	// Require SP_DEBUG for Log macros.
	#define SP_WARNING_PLAN_GEN_FAILED SP_DEBUG
#endif