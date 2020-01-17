#include <SPlanner/Components/LODs/SP_LODComponent.h>

#include <GameFramework/PlayerController.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_LODComponent::USP_LODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true; // Use InitializeComponent().

	// Default active radius.
	SphereRadius = 5000.0f;

	// Set overlaps.
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

bool USP_LODComponent::GetIsInRange() const
{
	return bIsInRange;
}

void USP_LODComponent::OnEnterRange_Internal_Implementation()
{
	bIsInRange = true;
}
void USP_LODComponent::OnExitRange_Internal_Implementation()
{
	bIsInRange = false;
}

void USP_LODComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check for camera tag.
	if (OtherComp->ComponentTags.Find("Camera") == INDEX_NONE)
		return;

	OnEnterRange.Broadcast();
}
void USP_LODComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// Check for camera tag.
	if (OtherComp->ComponentTags.Find("Camera") == INDEX_NONE)
		return;

	OnExitRange.Broadcast();
}

void USP_LODComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnComponentBeginOverlap.AddDynamic(this, &USP_LODComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USP_LODComponent::OnEndOverlap);

	OnEnterRange.AddDynamic(this, &USP_LODComponent::OnEnterRange_Internal);
	OnExitRange.AddDynamic(this, &USP_LODComponent::OnExitRange_Internal);
}
void USP_LODComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	OnComponentBeginOverlap.RemoveDynamic(this, &USP_LODComponent::OnBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &USP_LODComponent::OnEndOverlap);

	OnEnterRange.RemoveDynamic(this, &USP_LODComponent::OnEnterRange_Internal);
	OnExitRange.RemoveDynamic(this, &USP_LODComponent::OnExitRange_Internal);
}

float USP_LODComponent::GetDistanceFromCamera(AActor* Actor)
{
	SP_SRCHECK_NULLPTR(Actor, -1.0f)

	const UWorld* const World = Actor->GetWorld();
	SP_SRCHECK_NULLPTR(World, -1.0f)

	const APlayerController* const PlayerController = World->GetFirstPlayerController();
	SP_SRCHECK_NULLPTR(PlayerController, -1.0f)
	SP_SRCHECK_NULLPTR(PlayerController->PlayerCameraManager, -1.0f)

	return FVector::Distance(Actor->GetActorLocation(), PlayerController->PlayerCameraManager->GetCameraLocation());
}
float USP_LODComponent::GetDistanceFromCamera(UActorComponent* Component)
{
	SP_SRCHECK_NULLPTR(Component, -1.0f)

	return GetDistanceFromCamera(Component->GetOwner());
}