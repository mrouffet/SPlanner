#include <SPlanner/Actors/SP_Character.h>

#include <SPlanner/Actors/SP_AIController.h>

#include <SPlanner/Components/SP_ActionSetComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>
#include <SPlanner/Components/SP_InteractZoneComponent.h>
#include <SPlanner/Components/LODs/SP_PlannerLODComponent.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>(TEXT("ActionSet"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("Target"));

	InteractZone = CreateDefaultSubobject<USP_InteractZoneComponent>(TEXT("InteractZone"));
	InteractZone->SetupAttachment(RootComponent);

	PlannerLOD = CreateDefaultSubobject<USP_PlannerLODComponent>(TEXT("PlannerLOD"));
	PlannerLOD->SetupAttachment(RootComponent);
}