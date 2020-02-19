// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

USP_Target::USP_Target(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Position{}
{
}
USP_Target::USP_Target(FVTableHelper& Helper) :
	Super(Helper),
	Position{}
{
}

ESP_TargetState USP_Target::GetState() const
{
	return State;
}

bool USP_Target::IsPosition() const
{
	return State == ESP_TargetState::TS_Position;
}
bool USP_Target::IsActor() const
{
	return State == ESP_TargetState::TS_Actor || State == ESP_TargetState::TS_Player;
}
bool USP_Target::IsPlayer() const
{
	return State == ESP_TargetState::TS_Player;
}
bool USP_Target::IsPOI() const
{
	return State == ESP_TargetState::TS_POI;
}

const FVector& USP_Target::GetPosition() const
{
	SP_RCHECK(IsPosition(), Position, "Bad target!")

	return Position;
}
FVector USP_Target::GetAnyPosition() const
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
AActor* USP_Target::GetActor() const
{
	SP_RCHECK(IsActor(), nullptr, "Bad target!")

	return Actor;
}
AActor* USP_Target::GetAnyActor() const
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
		break;
	}

	return nullptr;
}
APawn* USP_Target::GetPlayer() const
{
	SP_RCHECK(IsPlayer(), nullptr, "Bad target!")

	return Player;
}
USP_POIComponent* USP_Target::GetPOI() const
{
	SP_RCHECK(IsPOI(), nullptr, "Bad target!")

	return POI;
}


void USP_Target::SetPosition(const FVector& InPosition)
{
	Position = InPosition;
	State = ESP_TargetState::TS_Position;

	OnTargetChange.Broadcast(this);
}
void USP_Target::SetActor(AActor* InActor)
{
	SP_CHECK_NULLPTR(InActor)

	Actor = InActor;
	State = ESP_TargetState::TS_Actor;

	OnTargetChange.Broadcast(this);
}
void USP_Target::SetPlayer(APawn* InPlayer)
{
	SP_CHECK_NULLPTR(InPlayer)

	Player = InPlayer;
	State = ESP_TargetState::TS_Player;

	OnTargetChange.Broadcast(this);
}
void USP_Target::SetPOI(USP_POIComponent* InPOI)
{
	SP_CHECK_NULLPTR(InPOI)

	POI = InPOI;
	State = ESP_TargetState::TS_POI;

	OnTargetChange.Broadcast(this);
}

bool USP_Target::IsValid() const
{
	return State != ESP_TargetState::TS_None;
}

void USP_Target::Clear()
{
	Position = FVector::ZeroVector;
	State = ESP_TargetState::TS_None;
}

void USP_Target::Reset(USP_AIBlackboardObject* OriginalObject)
{
	SP_CHECK_NULLPTR(OriginalObject)

	USP_Target* OriginalTarget = Cast<USP_Target>(OriginalObject);
	SP_CHECK(OriginalTarget, "OriginalTarget nullptr! OriginalObject [%s] is not of type USP_Target", *OriginalObject->GetName())

	State = OriginalTarget->State;
	Position = OriginalTarget->Position; // Copy largest object of union.
}