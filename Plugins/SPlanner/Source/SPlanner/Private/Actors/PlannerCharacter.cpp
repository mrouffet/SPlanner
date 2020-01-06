#include <Actors/PlannerCharacter.h>

#include <Actors/PlannerController.h>

#include <Components/PlannerComponent.h>
#include <Components/TargetComponent.h>
#include <Components/InteractZoneComponent.h>

ASP_PlannerCharacter::ASP_PlannerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_PlannerController::StaticClass();

	Planner = CreateDefaultSubobject<USP_PlannerComponent>(TEXT("Planner"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("Target"));

	InteractZone = CreateDefaultSubobject<USP_InteractZoneComponent>(TEXT("InteractZone"));
	Planner->InteractZone = InteractZone;
}