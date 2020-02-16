// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_CircleFormation.h>

#include <GameFramework/Actor.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_CircleFormation::USP_CircleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 10;
}

bool USP_CircleFormation::Compute(AActor* LeadActor, AActor* TargetActor, const TArray<USP_AIPlannerComponent*>& Planners, TArray<FVector>& Offsets)
{
	if (!Super::Compute(LeadActor, TargetActor, Planners, Offsets))
		return false;

	FVector BaseDirection;

	if (TargetActor && TargetActor != LeadActor)
		BaseDirection = (TargetActor->GetActorLocation() - LeadActor->GetActorLocation()).GetSafeNormal();
	else
		BaseDirection = LeadActor->GetActorForwardVector();

	if (ConstructionType == ESP_FormationConstructionType::FCT_Dichotomy)
	{
		float AngleStep = 360.0f / Planners.Num();

		for (int i = 0; i < Planners.Num(); ++i)
			Offsets.Add(FRotator(0.0f, AngleStep * i, 0.0f).RotateVector(BaseDirection) * Radius);
	}
	else if (ConstructionType == ESP_FormationConstructionType::FCT_PointByPoint)
	{
		float AngleStep = 180.0f / MaxNum; // 180: simplification of (360.0f / MaxNum) / 2.0f.
		float BaseAngle = Planners.Num() % 2 ? 0.0f : AngleStep;

		for (int i = 0; i < Planners.Num(); ++i)
		{
			float Yaw = i % 2 == 0 ? AngleStep * i : -AngleStep * (i + 1);
			Offsets.Add(FRotator(0.0f, BaseAngle + Yaw, 0.0f).RotateVector(BaseDirection) * Radius);
		}
	}
	else
	{
		SP_LOG(Warning, "ConstructionType not supported yet!")
		return false;
	}

	return true;
}