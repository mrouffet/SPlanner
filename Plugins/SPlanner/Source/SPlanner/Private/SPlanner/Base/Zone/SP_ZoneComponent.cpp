// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Zone/SP_ZoneComponent.h>

#include <Components/PrimitiveComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_ZoneComponent::USP_ZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
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

	OnEnter.Broadcast(Object);
}
void USP_ZoneComponent::RemoveObject_Implementation(UPrimitiveComponent* Object)
{
	SP_CHECK_NULLPTR(Object)

	Objects.Remove(Object);

	OnExit.Broadcast(Object);
}

void USP_ZoneComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	// Handle serialized in editor: force SetHandle call to bind events.
	if(Handle)
		SetHandle(Handle);
}
void USP_ZoneComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	// Ensure callback unbind.
	if (Handle)
		SetHandle(nullptr);
}

void USP_ZoneComponent::SetHandle(UPrimitiveComponent* Primitive)
{
	SP_CHECK(!GetOwner()->GetIsReplicated() || GetOwnerRole() != ROLE_SimulatedProxy, "Replicated zone should only be init on server!")

	if (Handle)
	{
		Handle->OnComponentBeginOverlap.RemoveDynamic(this, &USP_ZoneComponent::OnBeginOverlap);
		Handle->OnComponentEndOverlap.RemoveDynamic(this, &USP_ZoneComponent::OnEndOverlap);
	}

	Handle = Primitive;

	if (Handle)
	{
		Handle->OnComponentBeginOverlap.AddDynamic(this, &USP_ZoneComponent::OnBeginOverlap);
		Handle->OnComponentEndOverlap.AddDynamic(this, &USP_ZoneComponent::OnEndOverlap);
	}
}