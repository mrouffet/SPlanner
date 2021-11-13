// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_MISC_STATIC_PARAM_GUARD
#define SPLANNER_MISC_STATIC_PARAM_GUARD

#include <SP/Misc/Param/AParam.hpp>

/**
*	\file StaticParam.hpp
* 
*	\brief Static parameter type definition.
* 
*	\ingroup Misc
*	\{
*/

namespace SP
{
	/**
	*	Static Parameter Handle.
	* 
	*	\tparam T	Type of parameter to handle.
	*/
	template <typename T>
	class StaticParam : public AParam<T>
	{
	public:
		/// Handled value.
		T value = T();

		T Get() const override final
		{
			return value;
		}
	};
}

/** \} */

#endif // GUARD
