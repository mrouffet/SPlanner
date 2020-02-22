// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#if !UE_BUILD_SHIPPING

	#define SP_DEBUG 1

	#define SP_DEBUG_EDITOR WITH_EDITOR

	// Enable log macros.
	#define SP_DEBUG_LOG SP_DEBUG

	// Always log generated plan in console only.
	#define SP_LOG_PLAN 0

	// No Multithread in debug: easy breakpoint debug.
	#define SP_MULTITHREAD 0

	#define SP_WARNING_PLAN_GEN_FAILED 0

#else

	#define SP_DEBUG 0

	#define SP_DEBUG_EDITOR 0

	// Enable log macros.
	#define SP_DEBUG_LOG SP_DEBUG

	// Always log generated plan in console only: Do not require SP_DEBUG.
	#define SP_LOG_PLAN 0

	// TODO: FIX MULTITHREAD RANDOM
	#define SP_MULTITHREAD 0

	// Require SP_DEBUG for Log macros.
	#define SP_WARNING_PLAN_GEN_FAILED 0

#endif