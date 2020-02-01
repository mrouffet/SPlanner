// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_LODComponent::USP_LODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default active radius.
	SphereRadius = 10000.0f;

	Tags.Add("SP_Player");

	// LOD overlap only with pawns by default.
	SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
}

bool USP_LODComponent::IsInRange() const
{
	return Objects.Num();
}

float USP_LODComponent::GetLevel() const
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

void USP_LODComponent::AddObject_Implementation(UPrimitiveComponent* Object)
{
	Super::AddObject_Implementation(Object);

	if (Objects.Num() == 1)
		OnActive.Broadcast();
}
void USP_LODComponent::RemoveObject_Implementation(UPrimitiveComponent* Object)
{
	Super::RemoveObject_Implementation(Object);

	if (Objects.Num() == 0)
		OnInactive.Broadcast();
}