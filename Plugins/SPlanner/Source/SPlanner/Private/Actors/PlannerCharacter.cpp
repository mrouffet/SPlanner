#include <Actors/PlannerCharacter.h>

#include <Actors/PlannerController.h>

#include <Components/ActionSetComponent.h>
#include <Components/TargetComponent.h>
#include <Components/InteractZoneComponent.h>

ASP_PlannerCharacter::ASP_PlannerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_PlannerController::StaticClass();

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>(TEXT("ActionSet"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("Target"));

	InteractZone = CreateDefaultSubobject<USP_InteractZoneComponent>(TEXT("InteractZone"));
}