#include <SPlanner/Components/Zones/SP_ZoneComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_ZoneComponent::USP_ZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	// Default active radius.
	SphereRadius = 100.0f;

	// Set overlaps.
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void USP_ZoneComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	SP_CHECK_NULLPTR(OtherComp)
	SP_CHECK(Tags.Num(), "No tag to check!")

	bool bTagFound = false;

	// Check for tag.
	for(int i = 0; i < Tags.Num(); ++i)
	{
		if (OtherComp->ComponentTags.Find(Tags[i]) != INDEX_NONE)
		{
			bTagFound = true;
			break;
		}
	}

	if (!bTagFound)
		return;

	AddObject(OtherComp);
}
void USP_ZoneComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	SP_CHECK_NULLPTR(OtherComp)

	// Try to remove.
	if(Objects.Find(OtherComp) != INDEX_NONE)
		RemoveObject(OtherComp);
}

void USP_ZoneComponent::AddObject_Implementation(UPrimitiveComponent* Object)
{
	SP_CHECK_NULLPTR(Object)

	Objects.Add(Object);

	OnEnterRange.Broadcast(Object);
}
void USP_ZoneComponent::RemoveObject_Implementation(UPrimitiveComponent* Object)
{
	SP_CHECK_NULLPTR(Object)

	Objects.Remove(Object);

	OnExitRange.Broadcast(Object);
}

void USP_ZoneComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.AddDynamic(this, &USP_ZoneComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USP_ZoneComponent::OnEndOverlap);
}
void USP_ZoneComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnComponentBeginOverlap.RemoveDynamic(this, &USP_ZoneComponent::OnBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &USP_ZoneComponent::OnEndOverlap);
}