// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ATASK_GUARD
#define SPLANNER_GOAP_ATASK_GUARD

#include <memory>

#include <SP/GOAP/Base/Action/AAction.hpp>

#include <SP/GOAP/AI/Task/TaskState.hpp>


/**
*	\file ATask.hpp
* 
*	\brief Task base type definition.
* 
*	\ingroup AI
*	\{
*/

namespace SP
{
	namespace AI
	{
		/**
		*	\brief Task data base type.
		*	This type must be a data struct with no virtual functions (ensure correct delete).
		*	Data is instantiated and deleted by the task and is required to run the task.
		*/
		struct TaskData
		{
			/// Task begin state.
			bool bHasBegun = false;
		};

		/**
		*	\brief Task base type.
		*	Task is a <b>small behavior</b> executed by an AI.
		*	It is divided into three steps: Begin, Tick and End.
		*/
		class ATask : public AAction
		{
		protected:
			/**
			*	Begin the task.
			*	This is the first step to be called.
			* 
			*	\param[in] _data	Instantiated data.
			*/
			virtual bool Begin(TaskData* _data) const;

			/**
			*	End the task.
			*	This is the last step to be called, after Begin and Tick.
			* 
			*	\param[in] _state	Last tick state.
			*	\param[in] _data	Instantiated data.
			*/
			virtual bool End(TaskState _state, TaskData* _data) const;

			/**
			*	Update the task.
			*	This is called each frame, after Begin and before End (when the task is completed).
			* 
			*	\param[in] _deltaTime	delta time to update.
			*	\param[in] _data	Instantiated data.
			* 
			*	\return Tick state.
			*/
			virtual TaskState Tick(float _deltaTime, TaskData* _data) const;

		public:
			/**
			*	Instantiate data used by this task.
			* 
			*	\returns Instantiated data (or nullptr if not needed).
			*/
			virtual TaskData* InstantiateData() const;

			/**
			*	Delete previously instantiated data.
			* 
			*	\param[in] _data	Data to delete.
			*/
			virtual void DeleteData(TaskData* _data) const;

			/**
			*	Task "high level" Update.
			*	Calls Begin, Tick (until success/failure) and then End.
			* 
			*	\param[in] _deltaTime	delta time to update.
			*	\param[in] _data		Instantiated data.
			* 
			*	\return Task state.
			*/
			TaskState Update(float _deltaTime, TaskData* _data);

			/**
			*	Force this task to terminate even if still pending.
			* 
			*	\param[in] _data	Instantiated data.
			*/
			void Terminate(TaskData* _data);
		};
	}
}

/** \} */

#endif // GUARD
