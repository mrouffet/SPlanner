#include <Actors/SP_AIController.h>

#include <Components/Planners/SP_AIPlannerComponent.h>
#include <Components/SP_ActionSetComponent.h>
#include <Components/SP_TargetComponent.h>
#include <Components/SP_InteractZoneComponent.h>
#include <Components/LODs/SP_PlannerLODComponent.h>

FName ASP_AIController::PlannerComponentName(TEXT("Planner"));

ASP_AIController::ASP_AIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Planner = CreateOptionalDefaultSubobject<USP_AIPlannerComponent>(PlannerComponentName);
	Planner->bAutoRegisterInDirector = true;
}

void ASP_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Planner->ActionSet = Cast<USP_ActionSetComponent>(InPawn->GetComponentByClass(USP_ActionSetComponent::StaticClass()));
	Planner->Target = Cast<USP_TargetComponent>(InPawn->GetComponentByClass(USP_TargetComponent::StaticClass()));
	Planner->InteractZone = Cast<USP_InteractZoneComponent>(InPawn->GetComponentByClass(USP_InteractZoneComponent::StaticClass()));
	Planner->LOD = Cast<USP_PlannerLODComponent>(InPawn->GetComponentByClass(USP_PlannerLODComponent::StaticClass()));
}
void ASP_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	Planner->ActionSet = nullptr;
	Planner->Target = nullptr;
	Planner->InteractZone = nullptr;
	Planner->LOD = nullptr;
}