// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/Weights/Generators/LinearWeightGenerator.hpp>

namespace SP
{
	LinearWeightGenerator::LinearWeightGenerator(uint32_t _seed) :
		mDist{ 0.0, 1.0 }, // Random between 0 and 1.
		mEngine{ _seed != uint32_t(-1) ? _seed : std::random_device()() } // Use provided seed or generate random seed.
	{
	}

	float LinearWeightGenerator::Generate(const AWeight* _weight)
	{
		if (!_weight)
		{
			// TODO: Add log.
			return AWeight::defaultValue;
		}

		const float random = static_cast<float>(mDist(mEngine));

		return random * _weight->GetWeight();
	}
}
