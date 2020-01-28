#include <SPlanner/Goals/SP_SequenceWeightGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Goals/SP_WeightGoal.h>

#include <SPlanner/Components/Planners/SP_PlannerComponent.h>
#include <SPlanner/Components/Zones/SP_PlannerLODComponent.h>


const USP_Goal* USP_SequenceWeightGoal::GetExecutionGoal(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, nullptr)

	if(!SubGoals.Num())
	{
		SP_LOG(Warning, "No sub-goal found!")
		return this;
	}

	float LODLevel = Planner->GetLOD() ? Planner->GetLOD()->GetLevel() : -1.0f;

	struct FSP_WeightGoalPair
	{
		const USP_WeightGoal* Goal = nullptr;
		float Weight = -1.0f;
	};

	// Sort using weight.
	struct WeightGoalSortFunctor
	{
		// Decreasing weight sort.
		bool operator()(const FSP_WeightGoalPair& lhs, const FSP_WeightGoalPair& rhs) const
		{
			return lhs.Weight > rhs.Weight;
		}
	};

	TArray<FSP_WeightGoalPair> Pairs;

	for (int i = 0; i < SubGoals.Num(); ++i)
	{
		SP_CCHECK(SubGoals[i], "SubGoals [%d] in [%s] is nullptr!", i, *GetName())

		Pairs.Add(FSP_WeightGoalPair{ SubGoals[i], SubGoals[i]->Weight.Get(LODLevel) * FMath::FRand() });
	}

	Pairs.Sort(WeightGoalSortFunctor());

	return Pairs[0].Goal;
}