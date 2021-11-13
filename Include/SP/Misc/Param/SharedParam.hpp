// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_MISC_SHARED_PARAM_GUARD
#define SPLANNER_MISC_SHARED_PARAM_GUARD

#include <memory>

#include <SP/Misc/Param/AParam.hpp>

#include <SP/Collections/Debug>

namespace SP
{
	template <typename T>
	class SharedParam : public AParam<T>
	{
	public:
		std::shared_ptr<AParam<T>> shared = nullptr;

		SharedParam() = default;

		SharedParam(std::shared_ptr<AParam<T>> _shared) :
			shared{ _shared }
		{
		}

		T Get() const override final
		{
			SP_RCHECK(shared, "Shared nullptr!", Error, T());

			return shared->Get();
		}
	};
}

#endif // GUARD
