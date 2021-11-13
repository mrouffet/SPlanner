// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_STATIC_WEIGHT_GUARD
#define SPLANNER_GOAP_STATIC_WEIGHT_GUARD

#include <memory>

#include <SP/GOAP/Base/Action/Weights/AWeight.hpp>

namespace SP
{
	class StaticWeight : public AWeight
	{
	public:
		std::shared_ptr<float> weight = std::make_shared<float>(1.0f);

		StaticWeight() = default;
		StaticWeight(std::shared_ptr<float> _weight);

		float GetWeight() const override final;
	};
}

#endif // GUARD

