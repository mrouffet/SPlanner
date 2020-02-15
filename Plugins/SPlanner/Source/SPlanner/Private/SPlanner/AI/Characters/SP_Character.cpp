// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Characters/SP_Character.h>

#include <SPlanner/Base/Zones/SP_PlannerLODComponent.h>

#include <SPlanner/AI/POI/SP_POIZoneComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Controllers/SP_AIController.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Blackboard = CreateDefaultSubobject<USP_AIBlackboardComponent>(TEXT("SP_Blackboard"));

	POIZone = CreateDefaultSubobject<USP_POIZoneComponent>(TEXT("SP_POIZone"));
	POIZone->SetupAttachment(RootComponent);

	PlannerLOD = CreateDefaultSubobject<USP_PlannerLODComponent>(TEXT("SP_PlannerLOD"));
	PlannerLOD->SetupAttachment(RootComponent);
}