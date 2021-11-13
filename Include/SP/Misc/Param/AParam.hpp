// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_MISC_APARAM_GUARD
#define SPLANNER_MISC_APARAM_GUARD

#include <memory>

namespace SP
{
	template <typename T>
	class AParam
	{
	public:
		virtual ~AParam() = default;

		virtual T Get() const = 0;
	};

	template <typename T>
	using ParamPtr = std::shared_ptr<AParam<T>>;
}

#endif // GUARD