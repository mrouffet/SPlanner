// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationSet.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Formation/SP_Formation.h>

AActor* USP_FormationSet::GetLeadActor() const
{
	return LeadActor;
}
USP_LODComponent* USP_FormationSet::GetLeadLOD() const
{
	return LeadLOD;
}

void USP_FormationSet::SetLeadActor(AActor* NewLeadActor)
{
	SP_CHECK_NULLPTR(NewLeadActor)

	LeadActor = NewLeadActor;

	LeadLOD = Cast<USP_LODComponent>(LeadActor->GetComponentByClass(USP_LODComponent::StaticClass()));
}

bool USP_FormationSet::Add_Implementation(USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(LeadActor, false)

	Planners.Add(Planner);

	UpdateFormation();

	return true;
}
void USP_FormationSet::Remove_Implementation(USP_AIPlannerComponent* Planner)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(LeadActor)

	Planners.Remove(Planner);

	UpdateFormation();
}

void USP_FormationSet::ChangeFormation()
{
	SP_CHECK_NULLPTR(LeadActor)
	SP_BENCHMARK_SCOPE(ChangeFormation)

	// Select new formation.
	USP_Formation* NewFormation = SelectRandomFormation(FindAvailableFormations());

	// Random the same formation.
	if (CurrentFormation == NewFormation)
		return;

	// End previous.
	if (CurrentFormation)
	{
		CurrentFormation->OnEnd(this);

		// Set private cooldown.
		if (!CurrentFormation->IsCooldownShared())
			Cooldowns.FindOrAdd(CurrentFormation) = CurrentFormation->GetCooldown(LeadLOD ? LeadLOD->GetLevel() : -1.0f);
	}

	CurrentFormation = NewFormation;

	// Start new.
	if (CurrentFormation)
		CurrentFormation->OnStart(this);
}
void USP_FormationSet::UpdateFormation()
{
	SP_CHECK_NULLPTR(LeadActor)
	SP_BENCHMARK_SCOPE(UpdateFormation)

	TArray<USP_Formation*> AvailableFormations = FindAvailableFormations();
	
	if(AvailableFormations.Find(CurrentFormation) == INDEX_NONE ||								// Formation no longer available.
		(RandomChangeFormationRate >= 0.0f && FMath::FRand() <= RandomChangeFormationRate))		// Random rate.
		ChangeFormation();
}

TArray<USP_Formation*> USP_FormationSet::FindAvailableFormations() const
{
	TArray<USP_Formation*> Result;

	for (int i = 0; i < Formations.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Formations[i])

		if (!PredicateAvailable(Formations[i]))
			continue;

		// Not enought planner for this formation (max reached since Formations is sorted by MinNum).
		if (Planners.Num() < Formations[i]->GetMinNum())
			break;

		// Too much planner for this formation.
		if (Planners.Num() > Formations[i]->GetMaxNum())
			continue;

		Result.Add(Formations[i]);
	}

	return Result;
}
USP_Formation* USP_FormationSet::SelectRandomFormation(const TArray<USP_Formation*>& AvailableFormations) const
{
	SP_RCHECK_NULLPTR(LeadActor, nullptr)

	struct FSP_WeightFormation
	{
		/** The handled formation. */
		USP_Formation* Formation = nullptr;

		/** The generated weight. */
		float GeneratedWeight = -1.0f;
	};

	struct WeightFormationFunctor
	{
		bool operator()(const FSP_WeightFormation& Rhs, const FSP_WeightFormation& Lhs) const
		{
			return Rhs.GeneratedWeight > Lhs.GeneratedWeight;
		}
	};

	TArray<FSP_WeightFormation> WeightFormations;

	if (!AvailableFormations.Num())
		return nullptr;

	// Query Leader LOD Level.
	float LODLevel = LeadLOD ? LeadLOD->GetLevel() : -1.0f;

	for (int i = 0; i < AvailableFormations.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(AvailableFormations[i])

		WeightFormations.Add(FSP_WeightFormation{ AvailableFormations[i], AvailableFormations[i]->GetWeight(LODLevel) * FMath::FRand() });
	}

	WeightFormations.Sort(WeightFormationFunctor{});

	return WeightFormations[0].Formation;
}

bool USP_FormationSet::PredicateAvailable_Implementation(USP_Formation* Formation) const
{
	SP_RCHECK_NULLPTR(Formation, false)
	SP_RCHECK_NULLPTR(LeadActor, false)

	// Do not check availability while in use.
	if (Formation == CurrentFormation)
		return true;

	if (!Formation->IsAvailable(this))
		return false;

	// Check cooldown.
	if (!Formation->IsCooldownShared())
	{
		const float* const CooldownPtr = Cooldowns.Find(Formation);
		return CooldownPtr ? *CooldownPtr - LeadActor->GetWorld()->GetTimeSeconds() <= 0.0f : true;
	}

	return true;
}

void USP_FormationSet::Reset_Implementation()
{
	LeadActor = nullptr;
	LeadLOD = nullptr;

	if (CurrentFormation)
	{
		CurrentFormation->OnEnd(this);

		CurrentFormation->Reset();
		CurrentFormation = nullptr;
	}

	Planners.Empty();
	Cooldowns.Empty();
}

#if SP_DEBUG_EDITOR
void USP_FormationSet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	struct FormationSortFunctor
	{
		bool operator()(const USP_Formation& Rhs, const USP_Formation& Lhs) const
		{
			if (Rhs.GetMinNum() == Lhs.GetMinNum())
				return Rhs.GetMaxNum() <= Lhs.GetMaxNum();

			return Rhs.GetMinNum() < Lhs.GetMinNum();
		}
	};

	if (PropertyChangedEvent.Property->GetName() == "Formations")
	{
		// Search for any null asset.
		for (int i = 0; i < Formations.Num(); ++i)
		{
			if (Formations[i] == nullptr)
				return;
		}

		// Sort call pointer dereference. All assets must be valid.
		Formations.Sort(FormationSortFunctor{});
	}
}
#endif