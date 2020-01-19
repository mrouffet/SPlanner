#include <SPlanner/Components/Zones/SP_LODComponent.h>

#include <GameFramework/PlayerController.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_LODComponent::USP_LODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default active radius.
	SphereRadius = 5000.0f;

	Tags.Add("SP_Player");
}

bool USP_LODComponent::IsInRange() const
{
	return Objects.Num();
}

float USP_LODComponent::GetClosestDistance() const
{
	if (!Objects.Num())
		return -1.0f;

	float closestSqrDist = FLT_MAX;

	for (int i = 0; i < Objects.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Objects[i])

		float currSqrDist = FVector::DistSquared(GetComponentLocation(), Objects[i]->GetComponentLocation());

		if (currSqrDist < closestSqrDist)
			closestSqrDist = currSqrDist;
	}

	return FMath::Sqrt(closestSqrDist);
}