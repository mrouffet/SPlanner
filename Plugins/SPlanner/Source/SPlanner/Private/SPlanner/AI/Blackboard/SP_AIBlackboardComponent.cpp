// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/BlackBoard/SP_AIBlackboardComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardAsset.h>
#include <SPlanner/AI/Blackboard/Object/SP_AIBlackboardObject.h>

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Bool.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Int.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Float.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Vector.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Rotator.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Name.h>
#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Object.h>

bool USP_AIBlackboardComponent::GetBool(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, false, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, false)

	const USP_AIBlackboardKey_Bool* const BoolKey = Cast<USP_AIBlackboardKey_Bool>(*KeyPtr);
	SP_RCHECK(BoolKey, false, "BoolKey nullptr! Entry with name [%s] is not registered as a bool!", *EntryName.ToString())
	
	return BoolKey->GetValue();
}
void USP_AIBlackboardComponent::SetBool(const FName& EntryName, bool Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Bool* const BoolKey = Cast<USP_AIBlackboardKey_Bool>(*KeyPtr);
	SP_CHECK(BoolKey, "BoolKey nullptr! Entry with name [%s] is not registered as a bool!", *EntryName.ToString())

	BoolKey->SetValue(Value);

	OnBoolValueChange.Broadcast(EntryName, Value);
}

int USP_AIBlackboardComponent::GetInt(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, 0, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, 0)

	const USP_AIBlackboardKey_Int* const IntKey = Cast<USP_AIBlackboardKey_Int>(*KeyPtr);
	SP_RCHECK(IntKey, 0, "IntKey nullptr! Entry with name [%s] is not registered as an int!", *EntryName.ToString())
	
	return IntKey->GetValue();
}
void USP_AIBlackboardComponent::SetInt(const FName& EntryName, int Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Int* const IntKey = Cast<USP_AIBlackboardKey_Int>(*KeyPtr);
	SP_CHECK(IntKey, "IntKey nullptr! Entry with name [%s] is not registered as an int!", *EntryName.ToString())

	IntKey->SetValue(Value);

	OnIntValueChange.Broadcast(EntryName, Value);
}

float USP_AIBlackboardComponent::GetFloat(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, 0.0f, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, 0.0f)

	const USP_AIBlackboardKey_Float* const FloatKey = Cast<USP_AIBlackboardKey_Float>(*KeyPtr);
	SP_RCHECK(FloatKey, 0.0f, "FloatKey nullptr! Entry with name [%s] is not registered as a float!", *EntryName.ToString())
	
	return FloatKey->GetValue();
}
void USP_AIBlackboardComponent::SetFloat(const FName& EntryName, float Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Float* const FloatKey = Cast<USP_AIBlackboardKey_Float>(*KeyPtr);
	SP_CHECK(FloatKey, "FloatKey nullptr! Entry with name [%s] is not registered as a float!", *EntryName.ToString())

	FloatKey->SetValue(Value);

	OnFloatValueChange.Broadcast(EntryName, Value);
}

const FVector& USP_AIBlackboardComponent::GetVector(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, FVector::ZeroVector, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, FVector::ZeroVector)

	const USP_AIBlackboardKey_Vector* const VectorKey = Cast<USP_AIBlackboardKey_Vector>(*KeyPtr);
	SP_RCHECK(VectorKey, FVector::ZeroVector, "VectorKey nullptr! Entry with name [%s] is not registered as a FVector!", *EntryName.ToString())
	
	return VectorKey->GetValue();
}
void USP_AIBlackboardComponent::SetVector(const FName& EntryName, const FVector& Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Vector* const VectorKey = Cast<USP_AIBlackboardKey_Vector>(*KeyPtr);
	SP_CHECK(VectorKey, "VectorKey nullptr! Entry with name [%s] is not registered as a FVector!", *EntryName.ToString())

	VectorKey->SetValue(Value);

	OnVectorValueChange.Broadcast(EntryName, Value);
}

const FRotator& USP_AIBlackboardComponent::GetRotator(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, FRotator::ZeroRotator, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, FRotator::ZeroRotator)

	const USP_AIBlackboardKey_Rotator* const RotatorKey = Cast<USP_AIBlackboardKey_Rotator>(*KeyPtr);
	SP_RCHECK(RotatorKey, FRotator::ZeroRotator, "RotatorKey nullptr! Entry with name [%s] is not registered as a FRotator!", *EntryName.ToString())
	
	return RotatorKey->GetValue();
}
void USP_AIBlackboardComponent::SetRotator(const FName& EntryName, const FRotator& Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Rotator* const RotatorKey = Cast<USP_AIBlackboardKey_Rotator>(*KeyPtr);
	SP_CHECK(RotatorKey, "RotatorKey nullptr! Entry with name [%s] is not registered as a FRotator!", *EntryName.ToString())

	RotatorKey->SetValue(Value);

	OnRotatorValueChange.Broadcast(EntryName, Value);
}

const FName& USP_AIBlackboardComponent::GetFName(const FName& EntryName) const
{
#if SP_DEBUG
	static const FName NoneName = "None";
#endif

	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, NoneName, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, NoneName)

	const USP_AIBlackboardKey_Name* const NameKey = Cast<USP_AIBlackboardKey_Name>(*KeyPtr);
	SP_RCHECK(NameKey, NoneName, "NameKey nullptr! Entry with name [%s] is not registered as a FName!", *EntryName.ToString())
	
	return NameKey->GetValue();
}
void USP_AIBlackboardComponent::SetFName(const FName& EntryName, const FName& Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Name* const NameKey = Cast<USP_AIBlackboardKey_Name>(*KeyPtr);
	SP_CHECK(NameKey, "NameKey nullptr! Entry with name [%s] is not registered as a FName!", *EntryName.ToString())

	NameKey->SetValue(Value);

	OnFNameValueChange.Broadcast(EntryName, Value);
}

USP_AIBlackboardObject* USP_AIBlackboardComponent::GetObject(const FName& EntryName) const
{
	const USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, nullptr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, nullptr)

	const USP_AIBlackboardKey_Object* const ObjectKey = Cast<USP_AIBlackboardKey_Object>(*KeyPtr);
	SP_RCHECK(ObjectKey, nullptr, "ObjectKey nullptr! Entry with name [%s] is not registered as an UObject!", *EntryName.ToString())
	
	return ObjectKey->GetValue();
}
void USP_AIBlackboardComponent::SetObject(const FName& EntryName, USP_AIBlackboardObject* Value)
{
	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_AIBlackboardKey_Object* const ObjectKey = Cast<USP_AIBlackboardKey_Object>(*KeyPtr);
	SP_CHECK(ObjectKey, "ObjectKey nullptr! Entry with name [%s] is not registered as an UObject!", *EntryName.ToString())

	ObjectKey->SetValue(Value);

	OnObjectValueChange.Broadcast(EntryName, Value);
}

void USP_AIBlackboardComponent::ResetValue(const FName& EntryName)
{
	USP_AIBlackboardAsset* const AIBlackboardAsset = Cast<USP_AIBlackboardAsset>(BlackboardAsset);
	SP_CHECK(AIBlackboardAsset, "AIBlackboardAsset nullptr! Blackboard asset must be of type USP_AIBlackboardAsset.")

	USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	// Find original Key.
	const TArray<FSP_AIBlackboardEntry>& Entries = AIBlackboardAsset->GetEntries();

	USP_AIBlackboardKey* OriginalKey = nullptr;

	for (int i = 0; i < Entries.Num(); ++i)
	{
		if (Entries[i].Name == EntryName)
		{
			OriginalKey = Entries[i].Key;
			break;
		}
	}

	SP_CHECK(OriginalKey, "Key with name %s not found in Blackboard %s", *EntryName.ToString(), *BlackboardAsset->GetName())

	(*KeyPtr)->ResetValue(OriginalKey);
}

void USP_AIBlackboardComponent::Reset_Implementation()
{
	USP_AIBlackboardAsset* const AIBlackboardAsset = Cast<USP_AIBlackboardAsset>(BlackboardAsset);
	SP_CHECK(AIBlackboardAsset, "AIBlackboardAsset nullptr! Blackboard asset must be of type USP_AIBlackboardAsset.")

	const TArray<FSP_AIBlackboardEntry>& Entries = AIBlackboardAsset->GetEntries();

	// Reset each entry.
	for (int i = 0; i < Entries.Num(); ++i)
	{
		USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(Entries[i].Name);

		SP_CCHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *Entries[i].Name.ToString())
		SP_CCHECK_NULLPTR(*KeyPtr)

		(*KeyPtr)->ResetValue(Entries[i].Key);
	}
}
void USP_AIBlackboardComponent::ResetPlanCancel_Implementation()
{
	USP_AIBlackboardAsset* const AIBlackboardAsset = Cast<USP_AIBlackboardAsset>(BlackboardAsset);
	SP_CHECK(AIBlackboardAsset, "AIBlackboardAsset nullptr! Blackboard asset must be of type USP_AIBlackboardAsset.")

	const TArray<FSP_AIBlackboardEntry>& Entries = AIBlackboardAsset->GetEntries();

	// Reset each entry.
	for (int i = 0; i < Entries.Num(); ++i)
	{
		SP_CCHECK(Entries[i].Key, "Key nullptr! Entry with name [%s] is nullptr!", *Entries[i].Name.ToString())

		if (!Entries[i].Key->ShouldResetOnPlanCancelled())
			continue;

		USP_AIBlackboardKey* const* const KeyPtr = Keys.Find(Entries[i].Name);

		SP_CCHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *Entries[i].Name.ToString())
		SP_CCHECK_NULLPTR(*KeyPtr)

		(*KeyPtr)->ResetValue(Entries[i].Key);
	}
}

void USP_AIBlackboardComponent::InitializeBlackboard_Implementation()
{
	Super::InitializeBlackboard_Implementation();

	SP_CHECK(Keys.Num() == 0, "Blackboard already init!")

	USP_AIBlackboardAsset* const AIBlackboardAsset = Cast<USP_AIBlackboardAsset>(BlackboardAsset);
	SP_CHECK(AIBlackboardAsset, "AIBlackboardAsset nullptr! Blackboard asset must be of type USP_AIBlackboardAsset.")

	const TArray<FSP_AIBlackboardEntry>& Entries = AIBlackboardAsset->GetEntries();

	for (int i = 0; i < Entries.Num(); ++i)
	{
		SP_CCHECK(!Keys.Find(Entries[i].Name), "Blackboard Entry already registered! 2 entries can't have the same name!")

		Keys.Add(Entries[i].Name, Entries[i].Key->CreateInstance(this));
	}
}
void USP_AIBlackboardComponent::UnInitializeBlackboard_Implementation()
{
	Super::UnInitializeBlackboard_Implementation();

	Keys.Empty();
}