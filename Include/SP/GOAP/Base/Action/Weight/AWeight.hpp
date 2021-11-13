// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AWEIGHT_GUARD
#define SPLANNER_GOAP_AWEIGHT_GUARD

#include <SP/Misc/Param/AParam.hpp>

namespace SP
{
	using AWeight = AParam<float>;

	static constexpr float gDefaultWeight = 1.0f;
}

#endif // GUARD
