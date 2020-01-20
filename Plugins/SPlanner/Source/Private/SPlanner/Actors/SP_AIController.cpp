#include <SPlanner/Actors/SP_AIController.h>

#include <SPlanner/Actors/SP_Director.h>

#include <SPlanner/Components/SP_ActionSetComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>

#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

#include <SPlanner/Components/Zones/SP_POIZoneComponent.h>
#include <SPlanner/Components/Zones/SP_ReactZoneComponent.h>
#include <SPlanner/Components/Zones/SP_PlannerLODComponent.h>

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
	Planner->POIZone = Cast<USP_POIZoneComponent>(InPawn->GetComponentByClass(USP_POIZoneComponent::StaticClass()));
	Planner->SetLOD(Cast<USP_PlannerLODComponent>(InPawn->GetComponentByClass(USP_PlannerLODComponent::StaticClass())));

	// Setup pawn react zones.
	TArray<USP_ReactZoneComponent*> ReactZones;
	InPawn->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

	for (int i = 0; i < ReactZones.Num(); ++i)
		ReactZones[i]->Planner = Planner;
}
void ASP_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	Planner->ActionSet = nullptr;
	Planner->Target = nullptr;
	Planner->POIZone = nullptr;
	Planner->SetLOD(nullptr);
}

void ASP_AIController::SetEnableBehavior(bool bEnable)
{
	// Already in good state.
	if (IsActorTickEnabled() == bEnable)
		return;

	if (Planner->bAutoRegisterInDirector)
	{
		if(bEnable)
			ASP_Director::Register(Planner);
		else
			ASP_Director::UnRegister(Planner);
	}

	Planner->SetEnableBehavior(bEnable);

	SetActorTickEnabled(bEnable);
}