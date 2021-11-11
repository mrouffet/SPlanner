// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_STATIC_WEIGHT_GUARD
#define SPLANNER_GOAP_STATIC_WEIGHT_GUARD

#include <SPlanner/GOAP/Base/Action/Weights/AWeight.hpp>

namespace SP
{
	class StaticWeight : public AWeight
	{
	public:
		float weight = 1.0f;

		float GetWeight() const override final;
	};
}

#endif // GUARD

