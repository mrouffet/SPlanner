// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_LINEAR_WEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_LINEAR_WEIGHT_DISTRIBUTION_GUARD

#include <SP/GOAP/Base/Action/Weight/AWeightDistribution.hpp>

/**
*	\file LinearWeightDistribution.hpp
* 
*	\brief Linear weight distribution type definition.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	\brief Linear weight distribution from parameter.
	*	No operations are applied to input weight.
	*/
	class LinearWeightDistribution : public AWeightDistribution
	{
	public:
		float Generate(const AWeight* _weight) override final;
	};
}

/** \} */

#endif // GUARD
