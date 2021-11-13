// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_COMPOND_TASK_GUARD
#define SPLANNER_GOAP_COMPOND_TASK_GUARD

#include <vector>

#include <SP/GOAP/AI/Task/ATask.hpp>

/**
*	\file CompoundTask.hpp
* 
*	\brief Compound Task type definition.
* 
*	\ingroup AI
*	\{
*/

namespace SP
{
	namespace AI
	{
		/**
		*	\brief Task composed of sub tasks.
		*	Hybrid GOAP/HTN (Hierarchical task network) planner.
		*	Sources:
		*	https://en.wikipedia.org/wiki/Hierarchical_task_network
		*	https://youtu.be/5RKDFeQ1Ndk
		*/
		class CompoundTask : public ATask
		{
		protected:
			bool Begin(TaskData* _data) const override final;
			bool End(TaskState _state, TaskData* _data) const override final;

			TaskState Tick(float _deltaTime, TaskData* _data) const override final;

		public:
			/// Sub tasks composition.
			std::vector<TaskPtr> tasks;

			TaskData* InstantiateData() const override final;
			void DeleteData(TaskData* _data) const override final;
		};


		/**
		*	\brief Compound Task associated data.
		*/
		struct CompoundTaskData : public TaskData
		{
			/// Current sub task index to update.
			size_t index = 0;

			/// Sub task associated data.
			TaskData* subData = nullptr;
		};
	}
}


/** \} */

#endif // GUARD
