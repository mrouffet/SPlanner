// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Misc/SP_AITriggerBoxComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Controller/SP_AIController.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_AITriggerBoxComponent::USP_AITriggerBoxComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void USP_AITriggerBoxComponent::NotifyActor(AActor* OtherActor)
{
	SP_CHECK_NULLPTR(OtherActor)

	if (const APawn* const Pawn = Cast<APawn>(OtherActor))
	{
		if (const ASP_AIController* const Controller = Cast<ASP_AIController>(Pawn->GetController()))
		{
			USP_AIPlannerComponent* const Planner = Controller->GetPlanner();
			SP_CHECK_NULLPTR(Planner)

			Planner->Notify(ESP_AIPlannerNotify::NP_Trigger);
		}
	}
}

void USP_AITriggerBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	SP_CHECK(bNotifyOnEnter, "Callback triggered while bNotifyOnEnter == false!")

	NotifyActor(OtherActor);
}
void USP_AITriggerBoxComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	SP_CHECK(bNotifyOnExit, "Callback triggered while bNotifyOnExit == false!")

	NotifyActor(OtherActor);
}

void USP_AITriggerBoxComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Server only
	if (GetOwnerRole() != ROLE_Authority)
		return;

	if (bNotifyOnEnter)
		OnComponentBeginOverlap.AddDynamic(this, &USP_AITriggerBoxComponent::OnBeginOverlap);

	if (bNotifyOnExit)
		OnComponentEndOverlap.AddDynamic(this, &USP_AITriggerBoxComponent::OnEndOverlap);
}
void USP_AITriggerBoxComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	// Server only
	if (GetOwnerRole() != ROLE_Authority)
		return;

	if (bNotifyOnEnter)
		OnComponentBeginOverlap.RemoveDynamic(this, &USP_AITriggerBoxComponent::OnBeginOverlap);

	if (bNotifyOnExit)
		OnComponentEndOverlap.RemoveDynamic(this, &USP_AITriggerBoxComponent::OnEndOverlap);
}