// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_THRESHOLD_PRE_CONDITION_GUARD
#define SPLANNER_GOAP_THRESHOLD_PRE_CONDITION_GUARD

#include <SP/GOAP/Base/Action/Conditions/APreCondition.hpp>

#include <SP/Collections/Debug>
#include <SP/Misc/Param/AParam.hpp>

namespace SP
{
	template <typename T = float>
	class ThresholdPreCondition : public APreCondition
	{
	public:
		ParamPtr<T> value;
		ParamPtr<T> threshold;

	protected:
		bool Validate_Internal(void* _userData) const override final
		{
			(void)_userData;

			SP_RCHECK(value, "Invalid value nullptr", Error, false);
			SP_RCHECK(threshold, "Invalid threshold nullptr", Error, false);

			return value->Get() > threshold->Get();
		}
	};
}

#endif // GUARD
