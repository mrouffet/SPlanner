// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_Formation.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

#include <SPlanner/Base/Action/SP_PlannerAction.h>
#include <SPlanner/Base/Decorator/SP_Decorator.h>

#include <SPlanner/AI/Formation/SP_FormationSet.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

ESP_FormationFocusType USP_Formation::GetFormationFocusType() const
{
	return FormationFocusType;
}

int USP_Formation::GetMinNum() const
{
	return MinNum;
}
int USP_Formation::GetMaxNum() const
{
	return MaxNum;
}

float USP_Formation::GetTickFrequency() const
{
	return TickFrequency;
}
float USP_Formation::GetLeadSqrDistThreshold() const
{
	return LeadSqrDistThreshold;
}

float USP_Formation::GetWeight(const UObject* Outer) const
{
	return Weight->Query(Outer);
}

bool USP_Formation::IsAvailable(const USP_FormationSet* FormationSet) const
{
	SP_RCHECK_NULLPTR(FormationSet, false)
	SP_RCHECK_NULLPTR(FormationSet->GetLeadActor(), false)

	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] is nulltpr!")
		SP_CCHECK(SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(),
			ESP_DecoratorFlag::DF_Availability), "Decorators[%d] flag must be ESP_DecoratorFlag::DF_Availability!")

		if (!Decorators[i]->Available_Validate(FormationSet))
			return false;
	}

	return true;

	return true;
}

FVector USP_Formation::ComputeBaseDirection(const FSP_FormationSetInfos& SetInfos)
{
	FVector BaseDirection;

	SP_RCHECK_NULLPTR(SetInfos.LeadActor, FVector())

	if (SetInfos.TargetActor && SetInfos.TargetActor != SetInfos.LeadActor)
		BaseDirection = (SetInfos.TargetActor->GetActorLocation() - SetInfos.LeadActor->GetActorLocation()).GetSafeNormal();
	else if (bUseLeadForwardAsReference || !SetInfos.Planners.Num())
		BaseDirection = SetInfos.LeadActor->GetActorForwardVector();
	else
	{
		SP_RCHECK_NULLPTR(SetInfos.Planners[0], SetInfos.LeadActor->GetActorForwardVector())
		SP_RCHECK_NULLPTR(SetInfos.Planners[0]->GetPawn(), SetInfos.LeadActor->GetActorForwardVector())

		BaseDirection = (SetInfos.Planners[0]->GetPawn()->GetActorLocation() - SetInfos.LeadActor->GetActorLocation()).GetSafeNormal();
	}

	return BaseDirection;
}
float USP_Formation::ComputeSignedAngle(const APawn* Pawn, const AActor* LeadActor, const FVector& BaseDirection)
{
	SP_RCHECK_NULLPTR(Pawn, 0.0f)
	SP_RCHECK_NULLPTR(LeadActor, 0.0f)

	return ComputeSignedAngle(Pawn->GetActorLocation(), LeadActor->GetActorLocation(), BaseDirection);
}
float USP_Formation::ComputeSignedAngle(const FVector& PawnLocation, const FVector& LeadLocation, const FVector& BaseDirection)
{
	const FVector Dir = (PawnLocation - LeadLocation).GetSafeNormal();

	const float DirDot = FVector::DotProduct(BaseDirection, Dir);
	const float UnsignedAngle = FMath::RadiansToDegrees(FMath::Acos(DirDot));

	const FVector Cross = FVector::CrossProduct(BaseDirection, Dir);
	const float Sign = FMath::Sign(FVector::DotProduct(Cross, FVector::UpVector));

	return UnsignedAngle * Sign;
}

void USP_Formation::Compute(FSP_FormationSetInfos SetInfos)
{
	SP_CHECK(SetInfos.Planners.Num() >= MinNum && SetInfos.Planners.Num() <= MaxNum, "Planners num not supported by formation!")

	FSP_FormationInfos Infos(SetInfos, ComputeBaseDirection(SetInfos));

	switch (ConstructionType)
	{
	default:
		SP_LOG(Warning, "Construction type not supported yet!")
	case ESP_FormationConstructionType::FCT_Dichotomy:
		ConstructDichotomy(Infos);
		break;
	case ESP_FormationConstructionType::FCT_PointByPoint:
		ConstructPointByPoint(Infos);
		break;
	}
}

void USP_Formation::ConstructDichotomy(FSP_FormationInfos& Infos)
{
	SP_CHECK_NULLPTR(Infos.LeadActor)
	SP_CHECK(GetClass() != USP_Formation::StaticClass(), "Method must be overridden in children!")

	// Compute angles.
	for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Infos.PlannerInfos[i].Planner)

		float Angle = ComputeSignedAngle(Infos.PlannerInfos[i].Planner->GetPawn(), Infos.LeadActor, Infos.BaseDirection);

		// Use [0, 360] interval.
		if (Angle < 0.0f)
			Angle += 360.0f;

		Infos.PlannerInfos[i].Angle = Angle;
	}

	Infos.PlannerInfos.Sort();
}
void USP_Formation::ConstructPointByPoint(FSP_FormationInfos& Infos)
{
	SP_CHECK_NULLPTR(Infos.LeadActor)
	SP_CHECK(GetClass() != USP_Formation::StaticClass(), "Method must be overridden in children!")

	// Compute angles.
	for (int i = 0; i < Infos.PlannerInfos.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Infos.PlannerInfos[i].Planner)

		Infos.PlannerInfos[i].Angle = ComputeSignedAngle(Infos.PlannerInfos[i].Planner->GetPawn(), Infos.LeadActor, Infos.BaseDirection);
	}

	Infos.PlannerInfos.Sort();
}

void USP_Formation::OnStart_Implementation(const USP_FormationSet* FormationSet)
{
	SP_CHECK_NULLPTR(FormationSet)
	SP_CHECK_NULLPTR(FormationSet->GetLeadActor())
}
void USP_Formation::OnEnd_Implementation(const USP_FormationSet* FormationSet)
{
	SP_CHECK_NULLPTR(FormationSet)
	SP_CHECK_NULLPTR(FormationSet->GetLeadActor())
}

void USP_Formation::Reset_Implementation()
{
}