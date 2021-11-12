// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AWEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_AWEIGHT_DISTRIBUTION_GUARD

#include <SPlanner/GOAP/Base/Action/Weights/AWeight.hpp>

namespace SP
{
	class AWeightDistribution
	{
	public:
		virtual float Generate(const AWeight* _weight) = 0;
	};
}

#endif // GUARD
