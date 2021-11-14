// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AWEIGHT_GUARD
#define SPLANNER_GOAP_AWEIGHT_GUARD

#include <SP/Misc/Param/AParam.hpp>

/**
*	\file AWeight.hpp
* 
*	\brief Action weight type definition.
* 
*	\ingroup GOAP
*	\{
*/

namespace SP
{
	/// Alias for weight parameter.
	using AWeight = AParam<float>;

	/// Default weight value.
	static constexpr float gDefaultWeight = 1.0f;
}


/** \} */

#endif // GUARD
