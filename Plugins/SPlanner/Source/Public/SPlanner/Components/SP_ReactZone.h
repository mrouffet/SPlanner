#pragma once

#include <Components/SphereComponent.h>
#include "SP_ReactZone.generated.h"

class USP_Goal;

class USP_PlannerComponent;

/**
 *	React zone implementation: switch goal on enter / exit.
 */
UCLASS(Blueprintable, BlueprintType, DisplayName = "ReactZone", ClassGroup = "SPlanner|ReactZone", meta=(BlueprintSpawnableComponent))
class SPLANNER_API USP_ReactZone : public USphereComponent
{
	GENERATED_BODY()
	
protected:
	/**
	*	The delay before applying ExitGoal on end overlap.
	*	use < 0 for no delay.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float DelayOnExit = -1.0f;

	/** Should save previous goal as ExitGoal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bSavePreviousGoal = false;

	/** The tag of the object to react with while overlapping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FName ReactObjectTag = "Player";

	/** The new goal to set when object with ReactObjectTag enter overlap.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Goal* EnterGoal = nullptr;

	/** The new goal to set when object with ReactObjectTag enter overlap.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner", meta=(EditCondition="!bSavePreviousGoal"))
	USP_Goal* ExitGoal = nullptr;

	/** Timer handle before setting ExitGoal after DelayOnExit. */
	FTimerHandle ExitTimer;

	/** Callback function bound to OnComponentBeginOverlap. */
	UFUNCTION(Category = "SPlanner|ReactZone")
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	/** Callback function bound to OnComponentEndOverlap. */
	UFUNCTION(Category = "SPlanner|ReactZone")
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	void InitializeComponent() override;
	void UninitializeComponent() override;

public:
	/** Cached planner component. */
	UPROPERTY(BlueprintReadWrite, Category = "SPlanner")
	USP_PlannerComponent* Planner = nullptr;

	USP_ReactZone(const FObjectInitializer& ObjectInitializer);
};
