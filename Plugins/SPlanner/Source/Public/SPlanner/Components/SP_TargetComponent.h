#pragma once

#include <Components/ActorComponent.h>
#include "SP_TargetComponent.generated.h"

class USP_POIComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_TargetEvent, USP_TargetComponent*, Target);

/**
*	Enum state of TargetComponent.
*/
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Target")
enum class ESP_TargetState : uint8
{
	TS_None				UMETA(Display = "None"),

	TS_Position			UMETA(Display = "Position"),
	TS_Actor			UMETA(Display = "Actor"),
	TS_Player			UMETA(Display = "Player"),
	TS_POI				UMETA(Display = "POI"),
};

/**
*	Implementation of planner target.
*/
UCLASS(BlueprintType, ClassGroup = "SPlanner|Target")
class SPLANNER_API USP_TargetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Target depending on State. */
	union
	{
		FVector Position;
		AActor* Actor;
		APawn* Player;
		USP_POIComponent* POI;
	};

	/** The current target state. */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	ESP_TargetState State = ESP_TargetState::TS_None;

public:
	/** Callback thrown when target is changed. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_TargetEvent OnTargetChange;

	USP_TargetComponent(const FObjectInitializer& ObjectInitializer);
	
	// DO NOT USE. Unreal internal use only.
	USP_TargetComponent(FVTableHelper& Helper);


	/** Getter of State. */
	ESP_TargetState GetState() const;

	/** Whether target is a position. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPosition() const;

	/** Whether target is an actor (or a player). */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsActor() const;

	/** Whether target is a player. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPlayer() const;

	/** Whether target is a position. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPOI() const;


	/** Getter of Position */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	const FVector& GetPosition() const;

	/** Getter of position for any kind of target. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	FVector GetAnyPosition() const;

	/** Getter of Actor */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	AActor* GetActor() const;

	/** Getter of any kind of actor */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	AActor* GetAnyActor() const;

	/** Getter of Actor as player */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	APawn* GetPlayer() const;

	/** Getter of TargetPOI */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	USP_POIComponent* GetPOI() const;


	/** Setter of TargetPosition */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPosition(const FVector& InPosition);

	/** Setter of Actor */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetActor(AActor* InActor);

	/** Setter of Actor as player */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPlayer(APawn* InPlayer);

	/** Setter of POI */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPOI(USP_POIComponent* InPOI);


	/** Return whether the target is valid. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsValid() const;

	/** Reset to EAITargetState::AITS_None. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void Clear();
};