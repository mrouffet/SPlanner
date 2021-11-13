// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_LINEAR_WEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_LINEAR_WEIGHT_DISTRIBUTION_GUARD

#include <SP/GOAP/Base/Action/Weight/AWeightDistribution.hpp>

namespace SP
{
	class LinearWeightDistribution : public AWeightDistribution
	{
	public:
		float Generate(const AWeight* _weight) override final;
	};
}

#endif // GUARD