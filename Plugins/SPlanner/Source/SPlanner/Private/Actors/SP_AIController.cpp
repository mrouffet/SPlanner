#include <Actors/SP_AIController.h>

#include <Components/SP_PlannerComponent.h>
#include <Components/SP_ActionSetComponent.h>
#include <Components/SP_TargetComponent.h>
#include <Components/SP_InteractZoneComponent.h>

ASP_AIController::ASP_AIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Planner = CreateDefaultSubobject<USP_PlannerComponent>(TEXT("Planner"));
	Planner->bAutoRegisterInAIDirector = true;
}

void ASP_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Planner->ActionSet = Cast<USP_ActionSetComponent>(InPawn->GetComponentByClass(USP_ActionSetComponent::StaticClass()));
	Planner->Target = Cast<USP_TargetComponent>(InPawn->GetComponentByClass(USP_TargetComponent::StaticClass()));
	Planner->InteractZone = Cast<USP_InteractZoneComponent>(InPawn->GetComponentByClass(USP_InteractZoneComponent::StaticClass()));
}
void ASP_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	Planner->ActionSet = nullptr;
	Planner->Target = nullptr;
	Planner->InteractZone = nullptr;
}