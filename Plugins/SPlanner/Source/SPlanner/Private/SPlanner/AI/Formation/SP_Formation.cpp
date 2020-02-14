// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_Formation.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Formation/Shapes/SP_FormationShape.h>

AActor* USP_Formation::GetLeadActor() const
{
	return LeadActor;
}
USP_LODComponent* USP_Formation::GetLeadLOD() const
{
	return LeadLOD;
}

void USP_Formation::InitLeadActor(AActor* NewLeadActor)
{
	SP_CHECK_NULLPTR(NewLeadActor)

	LeadActor = NewLeadActor;

	LeadLOD = Cast<USP_LODComponent>(LeadActor->GetComponentByClass(USP_LODComponent::StaticClass()));
}

bool USP_Formation::Add_Implementation(USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(LeadActor, false)

	Planners.Add(Planner);

	UpdateFormation();

	return true;
}
void USP_Formation::Remove_Implementation(USP_AIPlannerComponent* Planner)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(LeadActor)

	Planners.Remove(Planner);

	UpdateFormation();
}

void USP_Formation::ChangeShape()
{
	SP_CHECK_NULLPTR(LeadActor)
	SP_BENCHMARK_SCOPE(ChangeShape)

	// Select new shape.
	USP_FormationShape* NewShape = SelectRandomShapes(FindAvailableShapes());

	// Random the same shape.
	if (CurrentShape == NewShape)
		return;

	// End previous.
	if (CurrentShape)
	{
		CurrentShape->OnEnd(this);

		// Set private cooldown.
		if (!CurrentShape->IsCooldownShared())
			Cooldowns.FindOrAdd(CurrentShape) = CurrentShape->GetCooldown(LeadLOD ? LeadLOD->GetLevel() : -1.0f);
	}

	CurrentShape = NewShape;

	// Start new.
	if (CurrentShape)
		CurrentShape->OnStart(this);
}
void USP_Formation::UpdateFormation()
{
	SP_CHECK_NULLPTR(LeadActor)
	SP_BENCHMARK_SCOPE(UpdateFormation)

	TArray<USP_FormationShape*> AvailableShapes = FindAvailableShapes();
	
	if(AvailableShapes.Find(CurrentShape) == INDEX_NONE ||									// Shape no longer available.
		(RandomChangeFormationRate >= 0.0f && FMath::FRand() <= RandomChangeFormationRate))	// Random rate.
		ChangeShape();
}

TArray<USP_FormationShape*> USP_Formation::FindAvailableShapes() const
{
	TArray<USP_FormationShape*> Result;

	for (int i = 0; i < Shapes.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Shapes[i])

		if (!PredicateAvailable(Shapes[i]))
			continue;

		// Not enought planner for this formation (max reached since Shapes is sorted by MinNum).
		if (Planners.Num() < Shapes[i]->GetMinNum())
			break;

		// Too much planner for this formation.
		if (Planners.Num() > Shapes[i]->GetMaxNum())
			continue;

		Result.Add(Shapes[i]);
	}

	return Result;
}
USP_FormationShape* USP_Formation::SelectRandomShapes(const TArray<USP_FormationShape*>& AvailableShapes) const
{
	SP_RCHECK_NULLPTR(LeadActor, nullptr)

	struct FSP_WeightShape
	{
		/** The handled shape. */
		USP_FormationShape* Shape = nullptr;

		/** The generated weight. */
		float GeneratedWeight = -1.0f;
	};

	struct WeightShapeFunctor
	{
		bool operator()(const FSP_WeightShape& Rhs, const FSP_WeightShape& Lhs) const
		{
			return Rhs.GeneratedWeight > Lhs.GeneratedWeight;
		}
	};

	TArray<FSP_WeightShape> WeightShapes;

	if (!AvailableShapes.Num())
		return nullptr;

	// Query Leader LOD Level.
	float LODLevel = LeadLOD ? LeadLOD->GetLevel() : -1.0f;

	for (int i = 0; i < AvailableShapes.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(AvailableShapes[i])

		WeightShapes.Add(FSP_WeightShape{ AvailableShapes[i], AvailableShapes[i]->GetWeight(LODLevel) * FMath::FRand() });
	}

	WeightShapes.Sort(WeightShapeFunctor{});

	return WeightShapes[0].Shape;
}

bool USP_Formation::PredicateAvailable_Implementation(USP_FormationShape* Shape) const
{
	SP_RCHECK_NULLPTR(Shape, false)
	SP_RCHECK_NULLPTR(LeadActor, false)

	// Do not check availability while in use.
	if(Shape == CurrentShape)
		return true;

	if(!Shape->IsAvailable(this))
		return false;

	// Check cooldown.
	if (!Shape->IsCooldownShared())
	{
		const float* const CooldownPtr = Cooldowns.Find(Shape);
		return CooldownPtr ? *CooldownPtr - LeadActor->GetWorld()->GetTimeSeconds() <= 0.0f : true;
	}

	return true;
}

#if SP_DEBUG_EDITOR
void USP_Formation::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	struct ShapeSortFunctor
	{
		bool operator()(const USP_FormationShape& Rhs, const USP_FormationShape& Lhs) const
		{
			if (Rhs.GetMinNum() == Lhs.GetMinNum())
				return Rhs.GetMaxNum() <= Lhs.GetMaxNum();

			return Rhs.GetMinNum() < Lhs.GetMinNum();
		}
	};

	if (PropertyChangedEvent.Property->GetName() == "Shapes")
	{
		// Search for any null asset.
		for (int i = 0; i < Shapes.Num(); ++i)
		{
			if (Shapes[i] == nullptr)
				return;
		}

		// Sort call pointer dereference. All assets must be valid.
		Shapes.Sort(ShapeSortFunctor{});
	}
}
#endif