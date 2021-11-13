// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_MISC_APARAM_GUARD
#define SPLANNER_MISC_APARAM_GUARD

#include <memory>

/**
*	\file AParam.hpp
* 
*	\brief Parameter base type.
* 
*	\ingroup Misc
*	\{
*/

namespace SP
{
	/**
	*	\brief Generic Parameter base type.
	* 
	*	\tparam T	Type of parameter to handle.
	*/
	template <typename T>
	class AParam
	{
	public:
		virtual ~AParam() = default;

		/**
		*	Query handled parameter (child implementation).
		* 
		*	\return result parameter.
		*/
		virtual T Get() const = 0;
	};

	/**
	*	Alias for Param ptr (should always be shared).
	* 
	*	\tparam T	Type of parameter to handle.
	*/
	template <typename T>
	using ParamPtr = std::shared_ptr<AParam<T>>;
}

/** \} */

#endif // GUARD
