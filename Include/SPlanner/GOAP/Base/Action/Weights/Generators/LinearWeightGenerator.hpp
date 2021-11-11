// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_LINEAR_WEIGHT_GENERATOR_GUARD
#define SPLANNER_GOAP_LINEAR_WEIGHT_GENERATOR_GUARD

#include <random>

#include <SPlanner/GOAP/Base/Action/Weights/Generators/AWeightGenerator.hpp>

namespace SP
{
	/**
	*	Linerar Random Weight generator
	*	https://www.geogebra.org/calculator/zvceqebg
	*/

	class LinearWeightGenerator : public AWeightGenerator
	{
		std::default_random_engine mEngine;
		std::uniform_real_distribution<> mDist;

	public:
		LinearWeightGenerator(uint32_t _seed = uint32_t(-1));

		float Generate(const AWeight* _weight) override final;
	};
}

#endif // GUARD
