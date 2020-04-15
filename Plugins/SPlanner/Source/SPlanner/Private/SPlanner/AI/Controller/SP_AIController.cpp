// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Controller/SP_AIController.h>

#include <TimerManager.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zone/SP_ReactZoneComponent.h>

#include <SPlanner/AI/LOD/SP_AILODComponent.h>
#include <SPlanner/AI/POI/SP_POIZoneComponent.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Controller/SP_PathFollowingComponent.h>

FName ASP_AIController::PlannerComponentName(TEXT("Planner"));

ASP_AIController::ASP_AIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USP_PathFollowingComponent>(TEXT("PathFollowingComponent")))
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
	Planner->SetEnableBehavior(bEnable);
}
void ASP_AIController::Freeze(float Time)
{
	Planner->Freeze();
}
void ASP_AIController::UnFreeze()
{
	Planner->UnFreeze();
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
	
	Planner->Blackboard = Cast<USP_AIBlackboardComponent>(InPawn->GetComponentByClass(USP_AIBlackboardComponent::StaticClass()));
	Planner->SetLOD(Cast<USP_AILODComponent>(InPawn->GetComponentByClass(USP_AILODComponent::StaticClass())));

	// Setup pawn's react zones.
	TArray<USP_ReactZoneComponent*> ReactZones;
	InPawn->GetComponents<USP_ReactZoneComponent>(ReactZones, true);

	for (int i = 0; i < ReactZones.Num(); ++i)
		ReactZones[i]->SetPlanner(Planner);

	if (Planner->bStartActive && !Planner->IsBehaviorEnabled()) // Planner was waiting for possess (blackboard) to start.
		Planner->SetEnableBehavior(true);
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

	Planner->Blackboard = nullptr;
	Planner->SetLOD(nullptr);

	// UnPossess after reset (Pawn still valid).
	Super::OnUnPossess();
}