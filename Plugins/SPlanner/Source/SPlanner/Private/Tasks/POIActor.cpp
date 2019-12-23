#include <Actors/POIActor.h>

#include <Components/SphereComponent.h>

#include <Components/POIComponent.h>

ASP_POIActor::ASP_POIActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PointOfInterest = CreateDefaultSubobject<USP_POIComponent>("PointOfInterest");

	SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	SphereCollider->SetupAttachment(RootComponent);
}

USP_POIActionSet* ASP_POIActor::GetActionSet() const
{
	return PointOfInterest->GetActionSet();
}