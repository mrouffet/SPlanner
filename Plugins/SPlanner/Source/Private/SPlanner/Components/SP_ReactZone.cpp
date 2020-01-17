#include <SPlanner/Components/SP_ReactZone.h>

#include <TimerManager.h>

#include <SPlanner/Debug/SP_Debug.h>
#include <SPlanner/Components/Planners/SP_PlannerComponent.h>

USP_ReactZone::USP_ReactZone(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	// Default active radius.
	SphereRadius = 400.0f;

	// Set overlaps.
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void USP_ReactZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check for tag.
	if (OtherComp->ComponentTags.Find(ReactObjectTag) == INDEX_NONE)
		return;

	SP_CHECK_NULLPTR(Planner)

	if (EnterCooldown > 0.0f)
	{
		// Wait for cooldown.
		float WaitTime = GetWorld()->GetTimeSeconds() - CurrEnterCooldown;

		if (WaitTime > EnterCooldown)
		{
			// Call this again after wait time.
			GetWorld()->GetTimerManager().SetTimer(Timer,
				[this, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, &SweepResult]
				{
					OnBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
				},
				WaitTime - EnterCooldown + 0.01f, // ensure float precision.
				false);

			return;
		}

		CurrEnterCooldown = GetWorld()->GetTimeSeconds();
	}
	
	if(bSavePreviousGoal)
		ExitGoal = Planner->GetGoal();

	Planner->SetGoal(EnterGoal);

	GetWorld()->GetTimerManager().ClearTimer(Timer);
}
void USP_ReactZone::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// Check for tag.
	if (OtherComp->ComponentTags.Find(ReactObjectTag) == INDEX_NONE || ExitGoal)
		return;

	SP_CHECK_NULLPTR(Planner)

	// Timer waiting for enter cooldown.
	if(GetWorld()->GetTimerManager().IsTimerPending(Timer))
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		return;
	}

	if (DelayOnExit < 0.0f)
		Planner->SetGoal(ExitGoal);
	else
		GetWorld()->GetTimerManager().SetTimer(Timer, [this] { Planner->SetGoal(ExitGoal); }, DelayOnExit, false);
}

void USP_ReactZone::InitializeComponent()
{
	Super::InitializeComponent();

	OnComponentBeginOverlap.AddDynamic(this, &USP_ReactZone::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USP_ReactZone::OnEndOverlap);
}
void USP_ReactZone::UninitializeComponent()
{
	Super::UninitializeComponent();

	OnComponentBeginOverlap.RemoveDynamic(this, &USP_ReactZone::OnBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &USP_ReactZone::OnEndOverlap);
}