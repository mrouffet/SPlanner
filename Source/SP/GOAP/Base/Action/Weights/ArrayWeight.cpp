// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Weights/ArrayWeight.hpp>

#include <algorithm>

#include <Collections/Debug>

namespace SP
{
	float ArrayWeight::GetWeight() const
	{
		switch (method)
		{
			case SP::ArrayWeight::ComputeMethod::Average:
				return ComputeAverage();
			case SP::ArrayWeight::ComputeMethod::Sum:
				return ComputeSum();
			case SP::ArrayWeight::ComputeMethod::Median:
				return ComputeMedian();
			default:
			{
				SP_LOG("Compute Method not supported yet!", Error);
				return AWeight::defaultValue;
			}
		}
	}

	
	float ArrayWeight::ComputeAverage() const
	{
		SP_RCHECK(!weights.empty(), "Division by 0!", Error, AWeight::defaultValue);

		return ComputeSum() / weights.size();
	}
	
	float ArrayWeight::ComputeSum() const
	{
		float sum = 0.0f;

		for (auto& weight : weights)
		{
			SP_RCHECK(weight, "registered weight nullptr!", Error, AWeight::defaultValue);

			sum += weight->GetWeight();
		}

		return sum;
	}
	
	float ArrayWeight::ComputeMedian() const
	{
		std::vector<float> computedWeights;
		computedWeights.reserve(weights.size());

		for (auto& weight : weights)
		{
			SP_RCHECK(weight, "registered weight nullptr!", Error, AWeight::defaultValue);

			computedWeights.emplace_back(weight->GetWeight());
		}

		std::sort(computedWeights.begin(), computedWeights.end());


		if (weights.size() % 2)
		{
			// odd number
			const size_t index = weights.size() / 2 + 1;

			return computedWeights[index];
		}
		else
		{
			// even number

			const size_t index = weights.size() / 2;

			return (computedWeights[index] + computedWeights[index + 1]) / 2.0f;
		}
	}
}
