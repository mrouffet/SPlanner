#include <Components/POIInteractZoneComponent.h>

#include <Debug/Debug.h>

#include <Components/POIComponent.h>

USP_POIInteractZoneComponent::USP_POIInteractZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SphereRadius = 100.0f;
}

const TArray<USP_POIComponent*>& USP_POIInteractZoneComponent::GetPOIs() const
{
	return POIs;
}

void USP_POIInteractZoneComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	SP_CHECK_NULLPTR(OtherComp)

	if(USP_POIComponent* POI = Cast<USP_POIComponent>(OtherComp))
		POIs.Add(POI);
}

void USP_POIInteractZoneComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	SP_CHECK_NULLPTR(OtherComp)

	if (USP_POIComponent* POI = Cast<USP_POIComponent>(OtherComp))
	{
#if SP_DEBUG
		if (!POIs.Remove(POI))
			SP_LOG(Error, "POI not previously added in list!");
#else
		POIs.Remove(POI);
#endif
	}
}

void USP_POIInteractZoneComponent::BeginPlay()
{
	Super::BeginPlay();

	SP_CHECK_OWNER()

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.AddDynamic(this, &USP_POIInteractZoneComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &USP_POIInteractZoneComponent::OnOverlapEnd);
}
void USP_POIInteractZoneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	SP_CHECK_OWNER()

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.RemoveDynamic(this, &USP_POIInteractZoneComponent::OnOverlapBegin);
	OnComponentEndOverlap.RemoveDynamic(this, &USP_POIInteractZoneComponent::OnOverlapEnd);
}