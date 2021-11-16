// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <Base/AI/SP_AIController.h>

#include <Base/AI/SP_AIPlannerComponent.h>
#include <Base/AI/SP_BlackboardComponent.h>

ASP_AIController::ASP_AIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlannerComponent = CreateDefaultSubobject<USP_AIPlannerComponent>(TEXT("PlannerComponent"));
	Blackboard = CreateDefaultSubobject<USP_BlackboardComponent>(TEXT("BlackboardComponent"));
}

bool ASP_AIController::RunPlanner(USP_AIPlannerAsset* SPAsset)
{
	return false;
}
