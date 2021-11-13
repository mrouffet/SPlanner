// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Weights/StaticWeight.hpp>

#include <Collections/Debug>

namespace SP
{
	StaticWeight::StaticWeight(std::shared_ptr<float> _weight) :
		weight{ _weight }
	{
	}

	float StaticWeight::GetWeight() const
	{
		SP_RCHECK(weight, "Weight nullptr!", Error, AWeight::defaultValue);

		return *weight;
	}
}
