#include <SPlanner/Actors/SP_AIController.h>

#include <SPlanner/Debug/SP_Debug.h>

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
	
	// Start active by default.
	Planner->bStartActive = true;
	Planner->bAutoRegisterInDirector = true;
}

USP_AIPlannerComponent* ASP_AIController::GetPlanner() const
{
	return Planner;
}

void ASP_AIController::SetEnableBehavior(bool bEnable)
{
	// Already in good state.
	if (IsActorTickEnabled() == bEnable)
		return;

	Planner->SetEnableBehavior(bEnable);

	SetActorTickEnabled(bEnable);
}

void ASP_AIController::OnAskPlan_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Planner == InPlanner, "Bad planner binding")
}

void ASP_AIController::OnPlanCancel_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Planner == InPlanner, "Bad planner binding")
}

void ASP_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Planner->OnAskPlan.AddDynamic(this, &ASP_AIController::OnAskPlan);
	Planner->OnPlanCancel.AddDynamic(this, &ASP_AIController::OnPlanCancel);
}

void ASP_AIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Planner->OnAskPlan.RemoveDynamic(this, &ASP_AIController::OnAskPlan);
	Planner->OnPlanCancel.RemoveDynamic(this, &ASP_AIController::OnPlanCancel);
}

void ASP_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Planner->ActionSet = Cast<USP_ActionSetComponent>(InPawn->GetComponentByClass(USP_ActionSetComponent::StaticClass()));
	Planner->Target = Cast<USP_TargetComponent>(InPawn->GetComponentByClass(USP_TargetComponent::StaticClass()));
	Planner->POIZone = Cast<USP_POIZoneComponent>(InPawn->GetComponentByClass(USP_POIZoneComponent::StaticClass()));
	Planner->SetLOD(Cast<USP_PlannerLODComponent>(InPawn->GetComponentByClass(USP_PlannerLODComponent::StaticClass())));

	// Setup pawn's react zones.
	TArray<USP_ReactZoneComponent*> ReactZones;
	InPawn->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

	for (int i = 0; i < ReactZones.Num(); ++i)
		ReactZones[i]->Planner = Planner;
}
void ASP_AIController::OnUnPossess()
{
	// Reset pawn's react zones.
	if (GetPawn())
	{
		TArray<USP_ReactZoneComponent*> ReactZones;
		GetPawn()->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

		for (int i = 0; i < ReactZones.Num(); ++i)
			ReactZones[i]->Planner = nullptr;
	}

	Planner->ActionSet = nullptr;
	Planner->Target = nullptr;
	Planner->POIZone = nullptr;
	Planner->SetLOD(nullptr);

	// UnPossess after reset (Pawn still valid).
	Super::OnUnPossess();
}