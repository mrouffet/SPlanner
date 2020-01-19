#include <SPlanner/Actors/SP_Character.h>

#include <SPlanner/Actors/SP_AIController.h>

#include <SPlanner/Components/SP_ActionSetComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>
#include <SPlanner/Components/Zones/SP_POIZoneComponent.h>
#include <SPlanner/Components/Zones/SP_PlannerLODComponent.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>(TEXT("ActionSet"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("Target"));

	POIZone = CreateDefaultSubobject<USP_POIZoneComponent>(TEXT("POIZone"));
	POIZone->SetupAttachment(RootComponent);

	PlannerLOD = CreateDefaultSubobject<USP_PlannerLODComponent>(TEXT("PlannerLOD"));
	PlannerLOD->SetupAttachment(RootComponent);
}