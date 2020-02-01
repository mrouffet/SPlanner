#include <SPlanner/AI/POI/SP_POIActor.h>

#include <Components/SphereComponent.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

ASP_POIActor::ASP_POIActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	POI = CreateDefaultSubobject<USP_POIComponent>("SP_POI");
	POI->SetupAttachment(RootComponent);
}

USP_POIActionSet* ASP_POIActor::GetActionSet() const
{
	return POI->GetActionSet();
}