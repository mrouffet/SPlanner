// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AWEIGHT_GUARD
#define SPLANNER_GOAP_AWEIGHT_GUARD

namespace SP
{
	class AWeight
	{
	public:
		static constexpr float defaultValue = 1.0f;

		virtual ~AWeight() = default;

		virtual float GetWeight() const = 0;
	};
}

#endif // GUARD