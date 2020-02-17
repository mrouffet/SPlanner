// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationSet.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Zones/SP_LODComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>
#include <SPlanner/AI/Formation/SP_Formation.h>
#include <SPlanner/AI/Formation/SP_FormationFocusType.h>
#include <SPlanner/AI/Controllers/SP_AIController.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>
#include <SPlanner/AI/Director/SP_AIDirector.h>

#if SP_DEBUG_EDITOR
	#include <DrawDebugHelpers.h>

	#include <SPlanner/Miscs/SP_FlagHelper.h>
	#include <SPlannerEditor/Miscs/SP_EditorSettings.h>
#endif

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

bool USP_FormationSet::CheckAreContained(const TArray<USP_AIPlannerComponent*>& InPlanners, bool bShouldBeContained)
{
	for (int i = 0; i < InPlanners.Num(); ++i)
	{
		if ((Planners.Find(InPlanners[i]) == INDEX_NONE) == bShouldBeContained)
			return false;
	}

	return true;
}

void USP_FormationSet::InitPlannersData(const TArray<USP_AIPlannerComponent*>& InPlanners)
{
	for(int i = 0; i < InPlanners.Num(); ++i)
	{
		USP_AIPlannerComponent* Planner = InPlanners[i];
		SP_CCHECK_NULLPTR(Planner)

		SetFormationFocus(Planner);

		// Set lead actor target.
		USP_AIBlackboardComponent* const AIBlackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
		SP_CCHECK_NULLPTR(AIBlackboard)

		USP_Target* const Target = AIBlackboard->GetObject<USP_Target>(TargetEntryName);
		SP_CCHECK_NULLPTR(Target)
		Target->SetActor(LeadActor);
	}
}
void USP_FormationSet::UnInitPlannersData(const TArray<USP_AIPlannerComponent*>& InPlanners)
{
	for (int i = 0; i < InPlanners.Num(); ++i)
	{
		USP_AIPlannerComponent* Planner = InPlanners[i];
		SP_CCHECK_NULLPTR(Planner)

		ClearFormationFocus(Planner);

		USP_AIBlackboardComponent* const AIBlackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
		SP_CCHECK_NULLPTR(AIBlackboard)

		AIBlackboard->ResetValue(TargetEntryName);
		AIBlackboard->ResetValue(OffsetEntryName);
	}
}

bool USP_FormationSet::Add_Implementation(const TArray<USP_AIPlannerComponent*>& InPlanners)
{
	SP_RCHECK_NULLPTR(LeadActor, false)
	SP_RCHECK(InPlanners.Num(), false, "Empty InPlanners array!")

	SP_BENCHMARK_SCOPE(AddToFormation)

	// None of InPlanners must not be already contained in Planners.
	SP_RCHECK(CheckAreContained(InPlanners, false), false, "InPlanners already contained in Planners!")

	// Formation is full.
	if(!TryChangeFormation(Planners.Num() + InPlanners.Num()))
	{
#if SP_DEBUG_EDITOR
		if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Formation))
			SP_LOG_SCREEN(Display, FColor::Orange, "Formation full!")
#endif

		// TODO: Handle behavior: Try to add not whole InPlanners ?

		return false;
	}

	int PrevPlannersNum = Planners.Num();
	Planners.Append(InPlanners);

	// Init after formation my have change.
	InitPlannersData(InPlanners);

	UpdateFormation();

	if (PrevPlannersNum == 0)
		ASP_AIDirector::RegisterFormationSet(this);

#if SP_DEBUG_EDITOR
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Formation))
		SP_LOG_SCREEN(Display, FColor::Orange, "%s: %d", *CurrentFormation->GetName(), Planners.Num())
#endif

	return true;
}
bool USP_FormationSet::Remove_Implementation(const TArray<USP_AIPlannerComponent*>& InPlanners)
{
	SP_RCHECK_NULLPTR(LeadActor, false)
	SP_RCHECK(InPlanners.Num(), false, "Empty InPlanners array!")

	SP_BENCHMARK_SCOPE(RemoveFromFormation)

	// Each of InPlanners must be contained in Planners.
	SP_RCHECK(CheckAreContained(InPlanners, true), false, "InPlanners must be contained in Planners!")

	// Start by uninit, InPlanners won't be longer in formation.
	UnInitPlannersData(InPlanners);

	for (int i = 0; i < InPlanners.Num(); ++i)
		Planners.Remove(InPlanners[i]);

	// No more Planners in formation.
	if (Planners.Num() == 0)
	{
		if (CurrentFormation)
		{
			CurrentFormation->OnEnd(this);
			CurrentFormation = nullptr;
		}

		ASP_AIDirector::UnRegisterFormationSet(this);

		return true;
	}

	if (!TryChangeFormation(Planners.Num()))
	{
#if SP_DEBUG_EDITOR
		if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Formation))
			SP_LOG_SCREEN(Display, FColor::Orange, "Not enough planner in formation!")
#endif

		// TODO: Handle behavior.

		return false;
	}

	UpdateFormation();

#if SP_DEBUG_EDITOR
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Formation))
		SP_LOG_SCREEN(Display, FColor::Purple, "%s: %d", *CurrentFormation->GetName(), Planners.Num())
#endif

	return true;
}

bool USP_FormationSet::AddSingle(USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)

	return Add({ Planner });
}
bool USP_FormationSet::RemoveSingle(USP_AIPlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)

	return Remove({ Planner });
}

void USP_FormationSet::ApplyOffsets(const TArray<FVector>& Offsets)
{
	SP_CHECK(Planners.Num() == Offsets.Num(), "Formation [%s] offsets num doen't match planner num.", *CurrentFormation->GetName())

	for (int i = 0; i < Planners.Num(); ++i)
	{
		SP_CCHECK(Planners[i], "Planners[%d] nullptr!", i)

		USP_AIBlackboardComponent* const AIBlackboard = Planners[i]->GetBlackboard<USP_AIBlackboardComponent>();
		SP_CCHECK(AIBlackboard, "%s AIBlackboard nullptr!", *Planners[i]->GetName())

		AIBlackboard->SetVector(OffsetEntryName, Offsets[i]);

#if SP_DEBUG_EDITOR
		if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Formation))
		{
			float CurrDrawDebugTime = CurrentFormation->GetTickFrequency();

			if (CurrDrawDebugTime < 0.0f)
				CurrDrawDebugTime = DebugDrawTime;
			else if (CurrDrawDebugTime < 0.05f) // Minimum visible draw time.
				CurrDrawDebugTime = 0.05f;

			DrawDebugLine(LeadActor->GetWorld(), LeadActor->GetActorLocation(), LeadActor->GetActorLocation() + Offsets[i], DebugColor, false, CurrDrawDebugTime);
			DrawDebugPoint(LeadActor->GetWorld(), LeadActor->GetActorLocation() + Offsets[i], 10.0f, DebugColor, false, CurrDrawDebugTime);
		}
#endif
	}
}

bool USP_FormationSet::TryChangeFormation(int PlannerNum)
{
	// Check formation availability.
	TArray<USP_Formation*> AvailableFormations = FindAvailableFormations(PlannerNum);

	if (!CurrentFormation ||
		AvailableFormations.Find(CurrentFormation) == INDEX_NONE ||								// Formation no longer available.
		(RandomChangeFormationRate >= 0.0f && FMath::FRand() <= RandomChangeFormationRate))		// Random rate.
		return ChangeFormation_Internal(AvailableFormations);

	return true;
}

bool USP_FormationSet::ChangeFormation()
{
	return ChangeFormation_Internal(FindAvailableFormations(Planners.Num()));
}
bool USP_FormationSet::ChangeFormation_Internal(const TArray<USP_Formation*>& AvailableFormations)
{
	SP_RCHECK_NULLPTR(LeadActor, false)
	SP_BENCHMARK_SCOPE(ChangeFormation)

	// Select new formation.
	USP_Formation* NewFormation = SelectRandomFormation(AvailableFormations);

	if (!NewFormation)
		return false;

	// Random the same formation.
	if (CurrentFormation == NewFormation)
		return bCanSelectSameFormationWhenChange;

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
	CurrentFormation->OnStart(this);

	// Reset Focus.
	for (int i = 0; i < Planners.Num(); ++i)
		SetFormationFocus(Planners[i]);

	CurrTickTime = 0.0f;

	return true;
}

void USP_FormationSet::SetFormationFocus(USP_AIPlannerComponent* Planner)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(CurrentFormation)

	ASP_AIController* const AIController = Planner->GetController();
	SP_CHECK_NULLPTR(AIController)

	switch (CurrentFormation->GetFormationFocusType())
	{
	case ESP_FormationFocusType::FFT_None:
		break;
	case ESP_FormationFocusType::FFT_Lead:
	{
		AIController->SetFocus(LeadActor, EAIFocusPriority::Gameplay);

		break;
	}
	case ESP_FormationFocusType::FFT_Target:
	{
		SP_CHECK_NULLPTR(TargetActor)
		AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

		break;
	}
	case ESP_FormationFocusType::FFT_Clear:
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		
		break;
	}
	default:
		SP_LOG(Warning, "FormationFocus not supported yet!")
		break;
	}
}
void USP_FormationSet::ClearFormationFocus(USP_AIPlannerComponent* Planner)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(CurrentFormation)

	ASP_AIController* const AIController = Planner->GetController();
	SP_CHECK_NULLPTR(AIController)

	switch (CurrentFormation->GetFormationFocusType())
	{
	case ESP_FormationFocusType::FFT_None:
		break;
	case ESP_FormationFocusType::FFT_Lead:
	case ESP_FormationFocusType::FFT_Target:
	case ESP_FormationFocusType::FFT_Clear:
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

		break;
	}
	default:
		SP_LOG(Warning, "FormationFocus not supported yet!")
		break;
	}
}

TArray<USP_Formation*> USP_FormationSet::FindAvailableFormations(int PlannerNum) const
{
	TArray<USP_Formation*> Result;

	for (int i = 0; i < Formations.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Formations[i])

		if (!PredicateAvailable(Formations[i]))
			continue;

		// Not enough planner for this formation (max reached since Formations is sorted by MinNum).
		if (PlannerNum < Formations[i]->GetMinNum())
			break;

		// Too much planner for this formation.
		if (PlannerNum > Formations[i]->GetMaxNum())
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

	if (!AvailableFormations.Num())
		return nullptr;
	else if (AvailableFormations.Num() == 1)
		return AvailableFormations[0];

	TArray<FSP_WeightFormation> WeightFormations;

	// Query Leader LOD Level.
	float LODLevel = LeadLOD ? LeadLOD->GetLevel() : -1.0f;

	for (int i = 0; i < AvailableFormations.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(AvailableFormations[i])

		WeightFormations.Add(FSP_WeightFormation{ AvailableFormations[i], AvailableFormations[i]->GetWeight(LODLevel) * FMath::FRand() });
	}

	WeightFormations.Sort(WeightFormationFunctor{});

	// Can't select same formation.
	if (!bCanSelectSameFormationWhenChange && WeightFormations[0].Formation == CurrentFormation)
		return WeightFormations[1].Formation;

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

void USP_FormationSet::Tick(float DeltaSeconds)
{
	SP_CHECK_NULLPTR(CurrentFormation)

	if (CurrentFormation->GetTickFrequency() < 0.0f)
		return;

	CurrTickTime += DeltaSeconds;

	if (CurrTickTime < CurrentFormation->GetTickFrequency())
		return;

	CurrTickTime -= CurrentFormation->GetTickFrequency();

	UpdateFormation();
}
void USP_FormationSet::UpdateFormation()
{
	// No new formation found for planner num.
	SP_CHECK(CurrentFormation, "No available formation found for [%d] planners!", Planners.Num())

	TArray<FVector> Offsets;
	Offsets.Reserve(Planners.Num());

	CurrentFormation->Compute(FSP_FormationSetInfos{ Planners, Offsets, LeadActor, TargetActor });

	ApplyOffsets(Offsets);
}

void USP_FormationSet::Reset_Implementation()
{
	if (CurrentFormation)
	{
		CurrentFormation->OnEnd(this);
		CurrentFormation = nullptr;
	}

	// Reset each referenced formation.
	for (int i = 0; i < Formations.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Formations[i])
		Formations[i]->Reset();
	}

	LeadLOD = nullptr;
	LeadActor = nullptr;

	CurrTickTime = 0.0f;

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