// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Conditions/ACondition.hpp>

namespace SP
{
	bool ACondition::Validate(void* _userData) const
	{
		const bool res = Validate_Internal(_userData);

		return bReverse ? !res : res;
	}
}
