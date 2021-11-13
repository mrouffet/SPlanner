// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/Base/GOAPLinearPlanner.hpp>

#include <Collections/Debug>

namespace SP
{
	std::vector<const AAction*> GOAPLinearPlanner::Generate(const GenInfos& _infos)
	{
		std::vector<const AAction*> result;

		if (!Generate_Internal(_infos, result))
		{
			SP_LOG("Plan Generation Failed!", Warning);
		}

		return result;
	}

	bool GOAPLinearPlanner::Generate_Internal(const GenInfos& _infos, std::vector<const AAction*>& _outPlan, uint32_t _actionIndex, uint32_t _currDepth)
	{
		if (_currDepth > _infos.maxDepth)
			return false;

		for (uint32_t i = _actionIndex; i < (uint32_t)_infos.shuffledActions.size(); ++i)
		{
			const ActionHandle& action = *_infos.shuffledActions[i];

			if (action.ValidatePreConditions(_infos.userData) || action.ValidatePostConditions(_infos.userData))
			{
				_outPlan.emplace_back(action.action);

				if (action.bAchievePlan ||									// Check if action achieve plan.
					Generate_Internal(_infos, _outPlan, i + 1, _currDepth)) // Check if plan with this action can be achieved.
					return true;
				else
				{
					// Plan generation failed: remove this action from plan.
					_outPlan.pop_back();
				}
			}
		}

		return false;
	}
}
