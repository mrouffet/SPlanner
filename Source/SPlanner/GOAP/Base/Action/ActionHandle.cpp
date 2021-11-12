// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/ActionHandle.hpp>

namespace SP
{
	ActionHandle::~ActionHandle()
	{
		action = nullptr;
		weight = nullptr;


		for (auto cond : preconditions)
			delete cond;

		preconditions.clear();


		for (auto cond : postconditions)
			delete cond;

		postconditions.clear();
	}


	bool ActionHandle::ValidatePreConditions(void* _userData) const
	{
		for (auto cond : preconditions)
		{
			if (!cond->Validate(_userData))
				return false;
		}

		return true;
	}

	bool ActionHandle::ValidatePostConditions(void* _userData) const
	{
		for (auto cond : postconditions)
		{
			if (!cond->Validate(_userData))
				return false;
		}

		return true;
	}
}
