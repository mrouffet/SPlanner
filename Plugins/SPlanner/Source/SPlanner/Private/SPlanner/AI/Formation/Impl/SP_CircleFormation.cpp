// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/Impl/SP_CircleFormation.h>

#include <GameFramework/Actor.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_CircleFormation::USP_CircleFormation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxNum = 10;
}

void USP_CircleFormation::ConstructDichotomy(FSP_FormationInfos& Infos)
{
	Super::ConstructDichotomy(Infos);

	const float AngleImpl = 360.0f / Infos.PlannerInfos.Num();

	if (bUseLeadForwardAsReference)
	{
		// Simple contruct from forward.
		for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
		{
			float CurrAngleNoise = FMath::FRandRange(-1.0f, 1.0f) * AngleNoise;
			float CurrDistNoise = FMath::FRandRange(-1.0f, 1.0f) * DistanceNoise;

			Infos.PlannerInfos[i].Offset = FRotator(0.0f, AngleImpl * i + CurrAngleNoise, 0.0f).RotateVector(Infos.BaseDirection) * (Radius + CurrDistNoise);
		}

		return;
	}


	// Contruct with formation adaptation angle.
	TArray<float> OffsetAngles;
	OffsetAngles.Reserve(Infos.PlannerInfos.Num());

	float TotalAngleCorrection = 0.0f;

	for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Infos.PlannerInfos[i].Planner)
		SP_CCHECK_NULLPTR(Infos.PlannerInfos[i].Planner->GetPawn())

		// Desired angle.
		float Angle = AngleImpl * i;

		// Correction from planner input angle.
		float AngleDiff = Infos.PlannerInfos[i].Angle - Angle;
		float AngleCorrection = AngleDiff / Infos.PlannerInfos.Num(); // Correction will be spread among all planners.
		
		TotalAngleCorrection += AngleCorrection;

		OffsetAngles.Add(Angle);
	}

	// Apply computed angles and total correction to offsets.
	for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
	{
		SP_CCHECK(i < OffsetAngles.Num(), "OffsetAngles OutOfRange!")

		float CurrAngleNoise = FMath::FRandRange(-1.0f, 1.0f) * AngleNoise;
		float CurrDistNoise = FMath::FRandRange(-1.0f, 1.0f) * DistanceNoise;

		Infos.PlannerInfos[i].Offset =
			FRotator(0.0f, OffsetAngles[i] + TotalAngleCorrection + CurrAngleNoise, 0.0f).RotateVector(Infos.BaseDirection) * (Radius + CurrDistNoise);
	}
}
void USP_CircleFormation::ConstructPointByPoint(FSP_FormationInfos& Infos)
{
	Super::ConstructPointByPoint(Infos);

	float AngleImpl = 360.0f / MaxNum;
	float BaseAngle = -AngleImpl * (Infos.PlannerInfos.Num() - 1) / 2.0f;

	for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
	{
		float CurrAngleNoise = FMath::FRandRange(-1.0f, 1.0f) * AngleNoise;
		float CurrDistNoise = FMath::FRandRange(-1.0f, 1.0f) * DistanceNoise;

		Infos.PlannerInfos[i].Offset =
			FRotator(0.0f, BaseAngle + AngleImpl * i + CurrAngleNoise, 0.0f).RotateVector(Infos.BaseDirection) * (Radius + CurrDistNoise);
	}
}