#include <Actors/SP_Character.h>

#include <Actors/SP_AIController.h>

#include <Components/SP_ActionSetComponent.h>
#include <Components/SP_TargetComponent.h>
#include <Components/SP_InteractZoneComponent.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>(TEXT("ActionSet"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("Target"));

	InteractZone = CreateDefaultSubobject<USP_InteractZoneComponent>(TEXT("InteractZone"));
	InteractZone->SetupAttachment(RootComponent);
}