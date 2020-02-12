#include <SPlanner/AI/Controllers/SP_AIController.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_ActionSetComponent.h>
#include <SPlanner/Base/Zones/SP_ReactZoneComponent.h>
#include <SPlanner/Base/Zones//SP_PlannerLODComponent.h>

#include <SPlanner/AI/POI/SP_POIZoneComponent.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

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
	Planner->SetLOD(Cast<USP_PlannerLODComponent>(InPawn->GetComponentByClass(USP_PlannerLODComponent::StaticClass())));

	// Setup pawn's react zones.
	TArray<USP_ReactZoneComponent*> ReactZones;
	InPawn->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

	for (int i = 0; i < ReactZones.Num(); ++i)
		ReactZones[i]->SetPlanner(Planner);
}
void ASP_AIController::OnUnPossess()
{
	// Reset pawn's react zones.
	if (GetPawn())
	{
		TArray<USP_ReactZoneComponent*> ReactZones;
		GetPawn()->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

		for (int i = 0; i < ReactZones.Num(); ++i)
			ReactZones[i]->SetPlanner(nullptr);
	}

	Planner->ActionSet = nullptr;
	Planner->SetLOD(nullptr);

	// UnPossess after reset (Pawn still valid).
	Super::OnUnPossess();
}