#include <Actors/SP_POIActor.h>

#include <Components/SphereComponent.h>

#include <Components/SP_POIComponent.h>

ASP_POIActor::ASP_POIActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PointOfInterest = CreateDefaultSubobject<USP_POIComponent>("PointOfInterest");
	PointOfInterest->SetupAttachment(RootComponent);
}

USP_POIActionSet* ASP_POIActor::GetActionSet() const
{
	return PointOfInterest->GetActionSet();
}