// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_RANDOM_WEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_RANDOM_WEIGHT_DISTRIBUTION_GUARD

#include <random>

#include <SPlanner/GOAP/Base/Action/Weights/Distributions/AWeightDistribution.hpp>

namespace SP
{
	/**
	*	Linerar Random Weight distribution
	*	https://www.geogebra.org/calculator/zvceqebg
	*/

	class RandomWeightDistribution : public AWeightDistribution
	{
		std::default_random_engine mEngine;
		std::uniform_real_distribution<> mDist;

	public:
		RandomWeightDistribution(uint32_t _seed = uint32_t(-1));

		float Generate(const AWeight* _weight) override final;
	};
}

#endif // GUARD
