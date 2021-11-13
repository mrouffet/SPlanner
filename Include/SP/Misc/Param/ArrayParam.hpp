// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#ifndef SPLANNER_MISC_ARRAY_PARAM_GUARD
#define SPLANNER_MISC_ARRAY_PARAM_GUARD

#include <vector>
#include <memory>

#include <SP/Misc/Param/AParam.hpp>

namespace SP
{
	enum class ArrayComputeMethod
	{
		Average = 1,

		Sum,

		Median
	};

	template <typename T>
	class ArrayParam : public AParam<T>
	{
		T ComputeAverage() const;
		T ComputeSum() const;
		T ComputeMedian() const;

	public:
		std::vector<ParamPtr<T>> params;

		ArrayComputeMethod method = ArrayComputeMethod::Average;

		T Get() const override final;
	}
}

#include <SP/Misc/Param/ArrayParam.inl>

#endif // GUARD
