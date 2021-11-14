// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#ifndef SPLANNER_MISC_ARRAY_PARAM_GUARD
#define SPLANNER_MISC_ARRAY_PARAM_GUARD

#include <vector>
#include <memory>

#include <SP/Misc/Param/AParam.hpp>

/**
*	\file ArrayParam.hpp
* 
*	\brief Array of parameters type definition.
* 
*	\ingroup Misc
*	\{
*/

namespace SP
{
	/// Array Parameter compute methods.
	enum class ArrayComputeMethod
	{
		/// Compute average of registered parameters.
		Average = 1,

		/// Compute sum of registered parameters.
		Sum,

		/// Compute median of registered parameters.
		Median
	};

	/**
	*	Array of parameters
	* 
	*	\tparam T	Parameter type.
	*/
	template <typename T>
	class ArrayParam : public AParam<T>
	{
		T ComputeAverage() const;
		T ComputeSum() const;
		T ComputeMedian() const;

	public:
		/// Registered parameters.
		std::vector<ParamPtr<T>> params;

		/// Compute method.
		ArrayComputeMethod method = ArrayComputeMethod::Average;

		T Get() const override final;
	}
}

/** \} */

#include <SP/Misc/Param/ArrayParam.inl>

#endif // GUARD
