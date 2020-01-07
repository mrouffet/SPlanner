#include <Actors/PlannerController.h>

#include <Components/PlannerComponent.h>
#include <Components/ActionSetComponent.h>
#include <Components/TargetComponent.h>
#include <Components/InteractZoneComponent.h>

ASP_PlannerController::ASP_PlannerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Planner = CreateDefaultSubobject<USP_PlannerComponent>(TEXT("Planner"));
	Planner->bAutoRegisterInAIDirector = true;
}

void ASP_PlannerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Planner->ActionSet = Cast<USP_ActionSetComponent>(InPawn->GetComponentByClass(USP_ActionSetComponent::StaticClass()));
	Planner->Target = Cast<USP_TargetComponent>(InPawn->GetComponentByClass(USP_TargetComponent::StaticClass()));
	Planner->InteractZone = Cast<USP_InteractZoneComponent>(InPawn->GetComponentByClass(USP_InteractZoneComponent::StaticClass()));
}
void ASP_PlannerController::OnUnPossess()
{
	Super::OnUnPossess();

	Planner->ActionSet = nullptr;
	Planner->Target = nullptr;
	Planner->InteractZone = nullptr;
}