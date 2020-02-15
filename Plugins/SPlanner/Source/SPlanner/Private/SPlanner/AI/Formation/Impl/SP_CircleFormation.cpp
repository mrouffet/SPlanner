// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_CircleFormation.h>

#include <GameFramework/Actor.h>

USP_CircleFormation::USP_CircleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 10;
}

bool USP_CircleFormation::Compute(AActor* LeadActor, AActor* TargetActor, const TArray<USP_AIPlannerComponent*>& Planners, TArray<FVector>& Offsets)
{
	if (!Super::Compute(LeadActor, TargetActor, Planners, Offsets))
		return false;

	float AngleStep = 360.0f / Planners.Num();
	FVector BaseDirection = TargetActor ? (TargetActor->GetActorLocation() - LeadActor->GetActorLocation()).GetSafeNormal() : LeadActor->GetActorForwardVector();

	for (int i = 0; i < Planners.Num(); ++i)
		Offsets.Add(FRotator(0.0f, AngleStep * i, 0.0f).RotateVector(BaseDirection) * Radius);

	return true;
}