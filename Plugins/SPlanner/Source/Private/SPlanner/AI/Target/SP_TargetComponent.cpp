#include <SPlanner/AI/Target/SP_TargetComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

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

bool USP_TargetComponent::IsPosition() const
{
	return State == ESP_TargetState::TS_Position;
}
bool USP_TargetComponent::IsActor() const
{
	return State == ESP_TargetState::TS_Actor || State == ESP_TargetState::TS_Player;
}
bool USP_TargetComponent::IsPlayer() const
{
	return State == ESP_TargetState::TS_Player;
}
bool USP_TargetComponent::IsPOI() const
{
	return State == ESP_TargetState::TS_POI;
}

const FVector& USP_TargetComponent::GetPosition() const
{
	SP_RCHECK(IsPosition(), Position, "Bad target!")

	return Position;
}
FVector USP_TargetComponent::GetAnyPosition() const
{
	switch (State)
	{
	case ESP_TargetState::TS_Position:
		return Position;
	case ESP_TargetState::TS_Actor:
		SP_RCHECK_NULLPTR(Actor, FVector::ZeroVector)
		return Actor->GetActorLocation();
	case ESP_TargetState::TS_Player:
		SP_RCHECK_NULLPTR(Player, FVector::ZeroVector)
		return Player->GetActorLocation();
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
	SP_RCHECK(IsActor(), nullptr, "Bad target!")

	return Actor;
}
AActor* USP_TargetComponent::GetAnyActor() const
{
	switch (State)
	{
	case ESP_TargetState::TS_Actor:
		return Actor;
	case ESP_TargetState::TS_Player:
		return Player;
	case ESP_TargetState::TS_POI:
		SP_RCHECK_NULLPTR(POI, nullptr)
		return POI->GetOwner();
	default:
		SP_LOG(Error, "Bad target type!")
		break;
	}

	return nullptr;
}
APawn* USP_TargetComponent::GetPlayer() const
{
	SP_RCHECK(IsPlayer(), nullptr, "Bad target!")

	return Player;
}
USP_POIComponent* USP_TargetComponent::GetPOI() const
{
	SP_RCHECK(IsPOI(), nullptr, "Bad target!")

	return POI;
}


void USP_TargetComponent::SetPosition(const FVector& InPosition)
{
	Position = InPosition;
	State = ESP_TargetState::TS_Position;

	OnTargetChange.Broadcast(this);
}
void USP_TargetComponent::SetActor(AActor* InActor)
{
	SP_CHECK_NULLPTR(InActor)

	Actor = InActor;
	State = ESP_TargetState::TS_Actor;

	OnTargetChange.Broadcast(this);
}
void USP_TargetComponent::SetPlayer(APawn* InPlayer)
{
	SP_CHECK_NULLPTR(InPlayer)

	Player = InPlayer;
	State = ESP_TargetState::TS_Player;

	OnTargetChange.Broadcast(this);
}
void USP_TargetComponent::SetPOI(USP_POIComponent* InPOI)
{
	SP_CHECK_NULLPTR(InPOI)

	POI = InPOI;
	State = ESP_TargetState::TS_POI;

	OnTargetChange.Broadcast(this);
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