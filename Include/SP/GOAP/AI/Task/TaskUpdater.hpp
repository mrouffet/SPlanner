// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_TASK_UPDATER_GUARD
#define SPLANNER_GOAP_TASK_UPDATER_GUARD

#include <SP/GOAP/AI/Task/ATask.hpp>

/**
*	\file TaskUpdater.hpp
* 
*	\brief Helper class to update a task.
* 
*	\ingroup Task
*	\{
*/

namespace SP
{
	namespace AI
	{
		/**
		*	Helper class to update task.
		*/
		class TaskUpdater
		{
			/// Handled task.
			TaskPtr mHandle = nullptr;

			/// Handled task's data.
			void* mTaskData = nullptr;

			/// Has task begun state.
			bool bHasBegun = false;

		public:
			/**
			*	\brief Set task to handle.
			* 
			*	\param[in] _task	new task to handle.
			*/
			void SetTask(TaskPtr _task);

			/**
			*	\brief Update handled task.
			* 
			*	\param[in] _deltaTime	delta time value used for tick.
			* 
			*	\return handled task's output state.
			*/
			TaskState Update(float _deltaTime);
		};
	}
}


/** \} */

#endif // GUARD
