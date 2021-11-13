// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Conditions/ThresholdPreCondition.hpp>

namespace SP
{
	bool ThresholdPreCondition::Validate_Internal(void* _userData) const
	{
		(void)_userData;

		return value >= threshold;
	}
}
