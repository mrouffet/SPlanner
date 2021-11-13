// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <algorithm>

#include <SP/Collections/Debug>

namespace SP
{
	template <typename T>
	T ArrayParam<T>::ComputeAverage() const
	{
		SP_RCHECK(!params.empty(), "Division by 0!", Error, T());

		return ComputeSum() / params.size();
	}

	template <typename T>
	T ArrayParam<T>::ComputeSum() const
	{
		T sum = 0.0f;

		for (auto& param : params)
		{
			SP_RCHECK(param, "registered param nullptr!", Error, T());

			sum += param->Get();
		}

		return sum;
	}

	template <typename T>
	T ArrayParam<T>::ComputeMedian() const
	{
		std::vector<T> computedParams;
		computedParams.reserve(weights.size());

		for (auto& param : params)
		{
			SP_RCHECK(param, "registered param nullptr!", Error, T());

			computedParams.emplace_back(param->Get());
		}

		std::sort(computedParams.begin(), computedParams.end());


		if (params.size() % 2)
		{
			// odd number
			const size_t index = params.size() / 2 + 1;

			return computedParams[index];
		}
		else
		{
			// even number

			const size_t index = params.size() / 2;

			return (computedParams[index] + computedParams[index + 1]) / T(2);
		}
	}


	template <typename T>
	T ArrayParam<T>::Get() const
	{
		switch (method)
		{
			case ArrayComputeMethod::Average:
				return ComputeAverage();
			case ArrayComputeMethod::Sum:
				return ComputeSum();
			case ArrayComputeMethod::Median:
				return ComputeMedian();
			default:
			{
				SP_LOG("Compute Method not supported yet!", Error);
				return T();
			}
		}
	}
}
