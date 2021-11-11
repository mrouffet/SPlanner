// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/Action/ActionSet.hpp>

#include <algorithm>

#include <Collections/Debug>

#include <GOAP/Base/Action/Weights/Generators/AWeightGenerator.hpp>

namespace SP
{
	struct WeightedAction
	{
		size_t index = 0u;

		float weight = 1.0f;

		bool operator<(const WeightedAction& _rhs)
		{
			return weight < _rhs.weight;
		}
	};

	std::vector<const ActionHandle*> ActionSet::Shuffle(AWeightGenerator* _gen) const
	{
		SP_RCHECK(_gen, "Invalid argument: AWeightGenerator nullptr!", Error, {});

		std::vector<WeightedAction> weightActions;
		weightActions.reserve(actionHandles.size());
		
		// Generate weight for index.
		for (size_t i = 0; i < actionHandles.size(); ++i)
			weightActions.emplace_back(WeightedAction{ i, _gen->Generate(actionHandles[i].weight.get()) });

		// Sort by weights.
		std::sort(weightActions.begin(), weightActions.end());


		std::vector<const ActionHandle*> result;
		result.reserve(actionHandles.size());

		for (auto& wAction : weightActions)
			result.emplace_back(&actionHandles[wAction.index]);

		return result;
	}
}
