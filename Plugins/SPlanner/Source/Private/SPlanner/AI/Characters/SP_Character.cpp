#include <SPlanner/AI/Characters/SP_Character.h>


#include <SPlanner/Base/Actions/SP_ActionSetComponent.h>
#include <SPlanner/Base/Zones/SP_PlannerLODComponent.h>

#include <SPlanner/AI/POI/SP_POIZoneComponent.h>
#include <SPlanner/AI/Target/SP_TargetComponent.h>
#include <SPlanner/AI/Controllers/SP_AIController.h>

ASP_Character::ASP_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = ASP_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>(TEXT("SP_ActionSet"));

	Target = CreateDefaultSubobject<USP_TargetComponent>(TEXT("SP_Target"));

	POIZone = CreateDefaultSubobject<USP_POIZoneComponent>(TEXT("SP_POIZone"));
	POIZone->SetupAttachment(RootComponent);

	PlannerLOD = CreateDefaultSubobject<USP_PlannerLODComponent>(TEXT("SP_PlannerLOD"));
	PlannerLOD->SetupAttachment(RootComponent);
}