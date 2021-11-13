// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_THRESHOLD_PRE_CONDITION_GUARD
#define SPLANNER_GOAP_THRESHOLD_PRE_CONDITION_GUARD

#include <SP/GOAP/Base/Action/Conditions/APreCondition.hpp>

namespace SP
{
	class ThresholdPreCondition : public APreCondition
	{
	protected:
		bool Validate_Internal(void* _userData) const override final;

	public:
		float value = 1.0f;
		float threshold = 0.5f;
	};
}

#endif // GUARD
