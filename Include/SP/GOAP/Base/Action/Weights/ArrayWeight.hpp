// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ARRAY_WEIGHT_GUARD
#define SPLANNER_GOAP_ARRAY_WEIGHT_GUARD

#include <vector>
#include <memory>

#include <SP/GOAP/Base/Action/Weights/AWeight.hpp>

namespace SP
{
	class ArrayWeight : public AWeight
	{
		float ComputeAverage() const;
		float ComputeSum() const;
		float ComputeMedian() const;

	public:
		enum class ComputeMethod
		{
			Average = 1,

			Sum,

			Median
		};

		std::vector<std::shared_ptr<AWeight>> weights;

		ComputeMethod method = ComputeMethod::Average;

		float GetWeight() const override final;
	};
}

#endif // GUARD
