#include <Actors/AIDirector.h>

#include <Debug/Debug.h>

#include <Components/PlannerComponent.h>
#include <Components/ActionSetComponent.h>

ASP_AIDirector* ASP_AIDirector::Instance = nullptr;

ASP_AIDirector::ASP_AIDirector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Planner = CreateDefaultSubobject<USP_PlannerComponent>("Planner");
	Planner->bAutoRegisterInAIDirector = false; // Should never set to true for AIDirector's planner.

	ActionSet = CreateDefaultSubobject<USP_ActionSetComponent>("ActionSet");
	Planner->ActionSet = ActionSet;
}

void ASP_AIDirector::BeginPlay()
{
	Super::BeginPlay();

	SP_CHECK(!Instance, "Bad Instance: Already set! An other AIDirector actor may be already in scene.")

	Instance = this;
}
void ASP_AIDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SP_CHECK(Instance == this, "Bad Instance: Try to reset an other instance! An other AIDirector actor may be already in scene.")

	Instance = nullptr;
}

void ASP_AIDirector::Register(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnRegister_Internal(InPlanner);
}
void ASP_AIDirector::UnRegister(USP_PlannerComponent* InPlanner)
{
	SP_CHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnUnRegister_Internal(InPlanner);
}

bool ASP_AIDirector::TryRegister(USP_PlannerComponent* InPlanner)
{
	if(!Instance)
		return false;

	Instance->OnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnRegister_Internal succeeded.
}
bool ASP_AIDirector::TryUnRegister(USP_PlannerComponent* InPlanner)
{
	if (!Instance)
		return false;

	Instance->OnUnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnUnRegister_Internal succeeded.
}

void ASP_AIDirector::OnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Planners.Find(Planner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())

	Planners.Add(InPlanner);
	OnRegister.Broadcast(InPlanner);
}
void ASP_AIDirector::OnUnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Instance->Planners.Find(Planner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())

	Planners.Remove(InPlanner);
	OnUnRegister.Broadcast(InPlanner);
}