// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_MISC_STATIC_PARAM_GUARD
#define SPLANNER_MISC_STATIC_PARAM_GUARD

#include <SP/Misc/Param/AParam.hpp>

namespace SP
{
	template <typename T>
	class StaticParam : public AParam<T>
	{
	public:
		T value = T();

		T Get() const override final
		{
			return value;
		}
	};
}

#endif // GUARD
