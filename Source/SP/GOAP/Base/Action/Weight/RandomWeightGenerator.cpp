// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Weight/RandomWeightDistribution.hpp>

#include <Collections/Debug>

namespace SP
{
	RandomWeightDistribution::RandomWeightDistribution(uint32_t _seed) :
		mDist{ 0.0, 1.0 }, // Random between 0 and 1.
		mEngine{ _seed != uint32_t(-1) ? _seed : std::random_device()() } // Use provided seed or generate random seed.
	{
	}

	float RandomWeightDistribution::Generate(const AWeight* _weight)
	{
		SP_RCHECK(_weight, "Weight nullptr", Error, gDefaultWeight);

		const float random = static_cast<float>(mDist(mEngine));

		return random * _weight->Get();
	}
}
