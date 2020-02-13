// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/AI/Formation/SP_Formation.h>

USP_FormationShape::USP_FormationShape(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set default cooldown value.
	Cooldown.Default = 0.0f;
}

int USP_FormationShape::GetMinNum() const
{
	return MinNum;
}
int USP_FormationShape::GetMaxNum() const
{
	return MaxNum;
}

float USP_FormationShape::GetWeight(float LODLevel) const
{
	return Weight.Get(LODLevel);
}
float USP_FormationShape::GetCooldown(float LODLevel) const
{
	return Cooldown.Get(LODLevel);
}

bool USP_FormationShape::IsCooldownShared() const
{
	return bShareCooldown;
}

bool USP_FormationShape::IsAvailable(const USP_Formation* Formation) const
{
	SP_RCHECK_NULLPTR(Formation, false)
	SP_RCHECK_NULLPTR(Formation->GetLeadActor(), false)

	if (bShareCooldown && SavedTimeCooldown > 0.0f)
		return SavedTimeCooldown - Formation->GetLeadActor()->GetWorld()->GetTimeSeconds() <= 0.0f;

	return true;
}

void USP_FormationShape::OnStart_Implementation(const USP_Formation* Formation)
{
	SP_CHECK_NULLPTR(Formation)
	SP_CHECK_NULLPTR(Formation->GetLeadActor())
}
void USP_FormationShape::OnEnd_Implementation(const USP_Formation* Formation)
{
	SP_CHECK_NULLPTR(Formation)
	SP_CHECK_NULLPTR(Formation->GetLeadActor())

	if(bShareCooldown)
		SavedTimeCooldown = GetWorld()->GetTimeSeconds() + GetCooldown(Formation->GetLeadLOD() ? Formation->GetLeadLOD()->GetLevel() : -1.0f);
}