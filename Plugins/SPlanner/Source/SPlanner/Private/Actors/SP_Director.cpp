#include <Actors/SP_Director.h>

#include <Debug/SP_Debug.h>

#include <Goal/SP_Goal.h>

#include <Components/Planners/SP_PlannerComponent.h>
#include <Components/SP_ActionSetComponent.h>

ASP_Director* ASP_Director::Instance = nullptr;

ASP_Director::ASP_Director(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// TODO: Add Director Planner.
	//Planner = CreateDefaultSubobject<USP_PlannerComponent>("Planner");
	//Planner->bAutoRegisterInDirector = false; // Should never set to true for AIDirector's planner.

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>("ActionSet");
	//Planner->ActionSet = ActionSet;
}

void ASP_Director::BeginPlay()
{
	Super::BeginPlay();

	SP_CHECK(!Instance, "Bad Instance: Already set! An other AIDirector actor may be already in scene.")

	Instance = this;
}
void ASP_Director::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SP_CHECK(Instance == this, "Bad Instance: Try to reset an other instance! An other AIDirector actor may be already in scene.")

	Instance = nullptr;
}

int ASP_Director::GetPlannerNum() const
{
	int Num = 0;

	for (auto it = GoalPlannersMap.begin(); it != GoalPlannersMap.end(); ++it)
		Num += it->Value.Num();

	return Num;
}
TArray<USP_PlannerComponent*> ASP_Director::QueryAllPlanners() const
{
	TArray<USP_PlannerComponent*> Planners;

	for (auto it = GoalPlannersMap.begin(); it != GoalPlannersMap.end(); ++it)
		Planners.Append(it->Value);

	return Planners;
}
TArray<USP_PlannerComponent*> ASP_Director::QueryAllPlanners()
{
	// Call const implementation.
	return static_cast<const ASP_Director*>(this)->QueryAllPlanners();
}
const TArray<USP_PlannerComponent*>& ASP_Director::QueryAllPlannersWithGoal(USP_Goal* Goal)
{
	TArray<USP_PlannerComponent*>* Planners = GoalPlannersMap.Find(Goal);

	// Goal not previously registered: add to map.
	if (!Planners)
		return GoalPlannersMap.Add(Goal, {});
	
	return *Planners;
}

ASP_Director* ASP_Director::GetInstance()
{
	SP_RCHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.", nullptr)

	return Instance;
}

void ASP_Director::Register(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnRegister_Internal(InPlanner);
}
void ASP_Director::UnRegister(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnUnRegister_Internal(InPlanner);
}

bool ASP_Director::TryRegister(USP_PlannerComponent* InPlanner)
{
	if(!Instance)
		return false;

	Instance->OnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnRegister_Internal succeeded.
}
bool ASP_Director::TryUnRegister(USP_PlannerComponent* InPlanner)
{
	if (!Instance)
		return false;

	Instance->OnUnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnUnRegister_Internal succeeded.
}

void ASP_Director::OnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)

	TArray<USP_PlannerComponent*>* Planners = GoalPlannersMap.Find(InPlanner->GetGoal());

	if (Planners)
	{
		SP_CHECK(Planners->Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())
		Planners->Add(InPlanner);
	}
	else
		GoalPlannersMap.Add(InPlanner->GetGoal(), { InPlanner });
	
	InPlanner->OnGoalChange.AddDynamic(this, &ASP_Director::OnRegistedPlannerGoalChange);

	OnRegister.Broadcast(InPlanner);
}
void ASP_Director::OnUnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)

	TArray<USP_PlannerComponent*>* Planners = GoalPlannersMap.Find(InPlanner->GetGoal());

	SP_CHECK_NULLPTR(Planners) // Should never be nullptr (previously added in OnRegister_Internal()).
	SP_CHECK(Planners->Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())

	InPlanner->OnGoalChange.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerGoalChange);

	Planners->Remove(InPlanner);
	OnUnRegister.Broadcast(InPlanner);
}

void ASP_Director::OnRegistedPlannerGoalChange(USP_PlannerComponent* InPlanner, USP_Goal* OldGoal, USP_Goal* NewGoal)
{
	SP_CHECK_NULLPTR(InPlanner)

	// Find previous goal registered.
	TArray<USP_PlannerComponent*>* Planners = GoalPlannersMap.Find(OldGoal);
	
	SP_CHECK_NULLPTR(Planners) // Should never be nullptr (previously added in OnRegister_Internal()).
	SP_CHECK(Planners->Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered with goal [ %s ]!",
		*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), (OldGoal ? *OldGoal->GetName() : *FString("None")))

	Planners->Remove(InPlanner);

	// Register planner with new goal.
	Planners = GoalPlannersMap.Find(NewGoal);

	if (Planners)
	{
		SP_CHECK(Planners->Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())
		Planners->Add(InPlanner);
	}
	else
		GoalPlannersMap.Add(NewGoal, { InPlanner });
}