#include <Actors/PlannerCharacter.h>

#include <Actors/PlannerController.h>
#include <Components/PlannerComponent.h>

ASP_PlannerCharacter::ASP_PlannerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_PlannerController::StaticClass();

	// Planner
	Planner = CreateDefaultSubobject<USP_PlannerComponent>(TEXT("Planner"));
}