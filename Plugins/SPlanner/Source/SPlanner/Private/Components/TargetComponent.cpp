#include <Components/TargetComponent.h>

#include <Debug/Debug.h>

#include <Components/POIComponent.h>

USP_TargetComponent::USP_TargetComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Position{}
{
}
USP_TargetComponent::USP_TargetComponent(FVTableHelper& Helper) :
	Super(Helper),
	Position{}
{
}

ESP_TargetState USP_TargetComponent::GetState() const
{
	return State;
}

const FVector& USP_TargetComponent::GetPosition() const
{
	SP_RCHECK(State == ESP_TargetState::TS_Position, "Bad target!", Position)

	return Position;
}
FVector USP_TargetComponent::GetAnyPosition() const
{
	switch (State)
	{
	case ESP_TargetState::TS_Position:
		return Position;
	case ESP_TargetState::TS_Actor:
	case ESP_TargetState::TS_Player:
		SP_RCHECK_NULLPTR(Actor, FVector::ZeroVector)
		return Actor->GetActorLocation();
	case ESP_TargetState::TS_POI:
		SP_RCHECK_NULLPTR(POI, FVector::ZeroVector)
		SP_RCHECK_NULLPTR(POI->GetOwner(), FVector::ZeroVector)
		return POI->GetOwner()->GetActorLocation();
	default:
		SP_LOG(Error, "Unknown target state!")
		return FVector::ZeroVector;
	}
}
AActor* USP_TargetComponent::GetActor() const
{
	SP_RCHECK(State == ESP_TargetState::TS_Actor, "Bad target!", nullptr)

	return Actor;
}
AActor* USP_TargetComponent::GetAnyActor() const
{
	if (State == ESP_TargetState::TS_Actor || State == ESP_TargetState::TS_Player)
		return Actor;
	else if (State == ESP_TargetState::TS_POI)
	{
		SP_RCHECK_NULLPTR(POI, nullptr)
		return POI->GetOwner();
	}

	SP_LOG(Error, "Bad target type!")

	return nullptr;
}
AActor* USP_TargetComponent::GetPlayer() const
{
	SP_RCHECK(State == ESP_TargetState::TS_Player, "Bad target!", nullptr)

	return Actor;
}
USP_POIComponent* USP_TargetComponent::GetPOI() const
{
	SP_RCHECK(State == ESP_TargetState::TS_POI, "Bad target!", nullptr)

	return POI;
}


void USP_TargetComponent::SetPosition(const FVector& InPosition)
{
	Position = InPosition;
	State = ESP_TargetState::TS_Position;
}
void USP_TargetComponent::SetActor(AActor* InActor)
{
	SP_CHECK_NULLPTR(InActor)

	Actor = InActor;
	State = ESP_TargetState::TS_Actor;
}
void USP_TargetComponent::SetPlayer(AActor* InPlayer)
{
	SP_CHECK_NULLPTR(InPlayer)

	Actor = InPlayer;
	State = ESP_TargetState::TS_Player;
}
void USP_TargetComponent::SetPOI(USP_POIComponent* InPOI)
{
	SP_CHECK_NULLPTR(InPOI)

	POI = InPOI;
	State = ESP_TargetState::TS_POI;
}

bool USP_TargetComponent::IsValid() const
{
	return State != ESP_TargetState::TS_None;
}

void USP_TargetComponent::Clear()
{
	Position = FVector::ZeroVector;
	State = ESP_TargetState::TS_None;
}