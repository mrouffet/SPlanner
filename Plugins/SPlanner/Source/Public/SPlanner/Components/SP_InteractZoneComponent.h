#pragma once

#include <Components/SphereComponent.h>
#include "SP_InteractZoneComponent.generated.h"

class USP_POIComponent;

/**
 *	Implementation of planner interact zone.
 *	Used to pickup POI action set.
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "InteractZone", ClassGroup = "SPlanner|InteractZone")
class SPLANNER_API USP_InteractZoneComponent : public USphereComponent
{
	GENERATED_BODY()
	
protected:
	/**
	*	Interactible POIs.
	*	POI currently overlapping this zone.
	*/
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_POIComponent*> POIs;

	/** Callback function bind to component begin overlap. */
	UFUNCTION(Category = "SPlanner|InteractZone")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** Callback function bind to component end overlap. */
	UFUNCTION(Category = "SPlanner|InteractZone")
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	USP_InteractZoneComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of POIs. */
	const TArray<USP_POIComponent*>& GetPOIs() const;
};