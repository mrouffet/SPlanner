// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Character/SP_Character.h>

#include <GameFramework/CharacterMovementComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/LOD/SP_AILODComponent.h>

#include <SPlanner/AI/POI/SP_POIZoneComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Controller/SP_AIController.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Blackboard = CreateDefaultSubobject<USP_AIBlackboardComponent>(TEXT("SP_Blackboard"));

	POIZone = CreateDefaultSubobject<USP_POIZoneComponent>(TEXT("SP_POIZone"));
	POIZone->SetupAttachment(RootComponent);

	LOD = CreateDefaultSubobject<USP_AILODComponent>(TEXT("SP_LOD"));

	// Avoid collision with other AI characters.
	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	SP_CHECK_NULLPTR(MovementComponent)

	// Set default avoidance.
	MovementComponent->bUseRVOAvoidance = true;
	MovementComponent->AvoidanceWeight = 0.5f;
	MovementComponent->AvoidanceConsiderationRadius = 250.0f;
}