// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AWEIGHT_DISTRIBUTION_GUARD
#define SPLANNER_GOAP_AWEIGHT_DISTRIBUTION_GUARD

#include <SP/GOAP/Base/Action/Weight/AWeight.hpp>

/**
*	\file AWeightDistribution.hpp
* 
*	\brief Weight Distribution base type.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/**
	*	Weight Distribution base type.
	* 
	*	Weight distribution is used for action shuffling before plan generation.
	*/
	class AWeightDistribution
	{
	public:
		/**
		*	Generate weight from parameter (child implementation).
		* 
		*	\param[in] _weight	input weight parameter.
		* 
		*	\return generated weight.
		*/
		virtual float Generate(const AWeight* _weight) = 0;
	};
}

/** \} */

#endif // GUARD
