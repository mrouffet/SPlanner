// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_TASK_STATE_GUARD
#define SPLANNER_GOAP_TASK_STATE_GUARD

/**
*	\file TaskState.hpp
* 
*	\brief Task state definition.
* 
*	\ingroup AI
*	\{
*/

namespace SP
{
	namespace AI
	{
		/**
		*	State of executed task.
		*/
		enum TaskState : uint8_t
		{
			/// Task succeeded.
			Success = 0,


			/**
			*	Task failed.
			*	Failure mask.
			*/
			Failure = 1 << 7,

			/// Begin task failed.
			BeginFailed = Failure | 1,

			/// End task failed.
			EndFailed = Failure | 2,


			/// Tick still pending.
			Pending = 2,


			/**
			*	Task being forced to end.
			*	Returning this value in Tick is <em>underfined behavior</em>.
			*/
			ForceEnd,
		};
	}
}

/** \} */

#endif // GUARD
