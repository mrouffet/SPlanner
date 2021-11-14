// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_RANDOM_WEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_RANDOM_WEIGHT_DISTRIBUTION_GUARD

#include <random>

#include <SP/GOAP/Base/Action/Weight/AWeightDistribution.hpp>

/**
*	\file RandomWeightDistribution.hpp
* 
*	\brief Random weight distribution type definition.
* 
*	Sources:
*	https://www.geogebra.org/calculator/zvceqebg
*
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	\brief Linerar Random Weight distribution.
	*	Random [0, 1] is applied to input weight.
	*/
	class RandomWeightDistribution : public AWeightDistribution
	{
		/// Random engine.
		std::default_random_engine mEngine;

		/// Random distribution.
		std::uniform_real_distribution<> mDist;

	public:
		/**
		*	\e Value constructor.
		* 
		*	\param[in] _seed	Seed used for random (random seed if not provided).
		*/
		RandomWeightDistribution(uint32_t _seed = uint32_t(-1));

		float Generate(const AWeight* _weight) override final;
	};
}

/** \} */

#endif // GUARD
