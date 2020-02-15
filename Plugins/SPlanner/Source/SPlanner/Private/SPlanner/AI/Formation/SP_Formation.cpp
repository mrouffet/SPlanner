// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_Formation.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/AI/Formation/SP_FormationSet.h>

USP_Formation::USP_Formation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set default cooldown value.
	Cooldown.Default = 0.0f;
}

int USP_Formation::GetMinNum() const
{
	return MinNum;
}
int USP_Formation::GetMaxNum() const
{
	return MaxNum;
}

float USP_Formation::GetWeight(float LODLevel) const
{
	return Weight.Get(LODLevel);
}
float USP_Formation::GetCooldown(float LODLevel) const
{
	return Cooldown.Get(LODLevel);
}

bool USP_Formation::IsCooldownShared() const
{
	return bShareCooldown;
}

bool USP_Formation::IsAvailable(const USP_FormationSet* FormationSet) const
{
	SP_RCHECK_NULLPTR(FormationSet, false)
	SP_RCHECK_NULLPTR(FormationSet->GetLeadActor(), false)

	if (bShareCooldown && SavedTimeCooldown > 0.0f)
		return SavedTimeCooldown - FormationSet->GetLeadActor()->GetWorld()->GetTimeSeconds() <= 0.0f;

	return true;
}

void USP_Formation::OnStart_Implementation(const USP_FormationSet* FormationSet)
{
	SP_CHECK_NULLPTR(FormationSet)
	SP_CHECK_NULLPTR(FormationSet->GetLeadActor())

	if (bShareCooldown)
		SavedTimeCooldown = GetWorld()->GetTimeSeconds() + GetCooldown(FormationSet->GetLeadLOD() ? FormationSet->GetLeadLOD()->GetLevel() : -1.0f);
}
void USP_Formation::OnEnd_Implementation(const USP_FormationSet* FormationSet)
{
	SP_CHECK_NULLPTR(FormationSet)
	SP_CHECK_NULLPTR(FormationSet->GetLeadActor())
}

void USP_Formation::Compute(const TArray<USP_AIPlannerComponent*>& Planners)
{
	SP_LOG(Error, "Method must be overridden in children!")
}