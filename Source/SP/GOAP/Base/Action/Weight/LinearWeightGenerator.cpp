// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Weight/LinearWeightDistribution.hpp>

#include <Collections/Debug>

namespace SP
{
	float LinearWeightDistribution::Generate(const AWeight* _weight)
	{
		SP_RCHECK(_weight, "Weight nullptr", Error, gDefaultWeight);

		return _weight->Get();
	}
}
