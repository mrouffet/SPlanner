// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/LOD/SP_LODComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_LODComponent::USP_LODComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool USP_LODComponent::IsLODActive() const
{
	return LODLevel >= 0.0f;
}
float USP_LODComponent::GetLODLevel() const
{
	return LODLevel;
}

void USP_LODComponent::ComputeLODLevel()
{
	SP_CHECK(Objects.Num(), "No Objects registered!")

	float ClosestSqrDist = FLT_MAX;
	FVector OwnerLocation = GetOwner()->GetActorLocation();

	for (int i = 0; i < Objects.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Objects[i])

		float CurrSqrDist = FVector::DistSquared(OwnerLocation, Objects[i]->GetComponentLocation());

		if (CurrSqrDist < ClosestSqrDist)
			ClosestSqrDist = CurrSqrDist;
	}

	float NewLODLevel = FMath::Sqrt(ClosestSqrDist);

	if (NewLODLevel > Radius)
	{
		// LOD out of range.


		if (LODLevel > 0.0f)
		{
			// Previously in range.

			LODLevel = -1.0f;
			OnInactive.Broadcast();
		}
	}
	else
	{
		// LOD in range.

		if (LODLevel < 0.0f)
		{
			// Previously out of range.
			
			LODLevel = NewLODLevel; // Set before broadcast.

			OnActive.Broadcast();
		}
		else
			LODLevel = NewLODLevel;
	}
}

void USP_LODComponent::FillObjects_Implementation()
{
	const APlayerController* const LocalPlayerController = GetWorld()->GetFirstPlayerController();
	SP_CHECK_NULLPTR(LocalPlayerController)

	const APawn* const LocalPlayerPawn = LocalPlayerController->GetPawn();
	SP_CHECK_NULLPTR(LocalPlayerPawn)
	SP_CHECK_NULLPTR(LocalPlayerPawn->GetRootComponent())

	Objects.Add(LocalPlayerPawn->GetRootComponent());
}

void USP_LODComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() != ROLE_SimulatedProxy)
		FillObjects();
	else
		SetComponentTickEnabled(false); // Never tick on simulated.
}
void USP_LODComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetOwnerRole() != ROLE_SimulatedProxy)
		Objects.Empty();
}

void USP_LODComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ComputeLODLevel();
}