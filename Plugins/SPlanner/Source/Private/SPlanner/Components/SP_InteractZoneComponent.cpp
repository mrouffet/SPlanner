#include <SPlanner/Components/SP_InteractZoneComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Components/SP_POIComponent.h>

USP_InteractZoneComponent::USP_InteractZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SphereRadius = 100.0f;
}

const TArray<USP_POIComponent*>& USP_InteractZoneComponent::GetPOIs() const
{
	return POIs;
}

void USP_InteractZoneComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
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

void USP_InteractZoneComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
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

void USP_InteractZoneComponent::BeginPlay()
{
	Super::BeginPlay();

	SP_CHECK_OWNER()

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.AddDynamic(this, &USP_InteractZoneComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &USP_InteractZoneComponent::OnOverlapEnd);
}
void USP_InteractZoneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	SP_CHECK_OWNER()

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.RemoveDynamic(this, &USP_InteractZoneComponent::OnOverlapBegin);
	OnComponentEndOverlap.RemoveDynamic(this, &USP_InteractZoneComponent::OnOverlapEnd);
}