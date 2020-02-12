#include <SPlanner/AI/BlackBoard/SP_BlackboardComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_BlackboardAsset.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Bool.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Int.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Float.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Vector.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Rotator.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Name.h>
#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Object.h>

void USP_BlackboardComponent::InitBlackboard(const USP_BlackboardAsset* BlackboardAssetSource)
{
	SP_CHECK_NULLPTR(BlackboardAssetSource)
	SP_CHECK(Keys.Num() == 0, "Blackboard already init!")

	const TArray<FSP_BlackboardEntry>& Entries = BlackboardAssetSource->GetEntries();

	for (int i = 0; i < Entries.Num(); ++i)
	{
		SP_CCHECK(!Keys.Find(Entries[i].Name), "Blackboard Entry already registered! 2 entries can't have the same name!")

		Keys.Add(Entries[i].Name, Entries[i].Key);
	}
}

bool USP_BlackboardComponent::GetBool(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, false, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, false)

	const USP_BlackboardKey_Bool* const BoolKey = Cast<USP_BlackboardKey_Bool>(*KeyPtr);
	SP_RCHECK(BoolKey, false, "BoolKey nullptr! Entry with name [%s] is not registered as a bool!", *EntryName.ToString())
	
	return BoolKey->GetValue();
}
void USP_BlackboardComponent::SetBool(const FName& EntryName, bool Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Bool* const BoolKey = Cast<USP_BlackboardKey_Bool>(*KeyPtr);
	SP_CHECK(BoolKey, "BoolKey nullptr! Entry with name [%s] is not registered as a bool!", *EntryName.ToString())

	BoolKey->SetValue(Value);
}

int USP_BlackboardComponent::GetInt(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, 0, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, 0)

	const USP_BlackboardKey_Int* const IntKey = Cast<USP_BlackboardKey_Int>(*KeyPtr);
	SP_RCHECK(IntKey, 0, "BoolKey nullptr! Entry with name [%s] is not registered as an int!", *EntryName.ToString())
	
	return IntKey->GetValue();
}
void USP_BlackboardComponent::SetInt(const FName& EntryName, int Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Int* const IntKey = Cast<USP_BlackboardKey_Int>(*KeyPtr);
	SP_CHECK(IntKey, "BoolKey nullptr! Entry with name [%s] is not registered as an int!", *EntryName.ToString())

	IntKey->SetValue(Value);
}

float USP_BlackboardComponent::GetFloat(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, 0.0f, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, 0.0f)

	const USP_BlackboardKey_Float* const FloatKey = Cast<USP_BlackboardKey_Float>(*KeyPtr);
	SP_RCHECK(FloatKey, 0.0f, "BoolKey nullptr! Entry with name [%s] is not registered as a float!", *EntryName.ToString())
	
	return FloatKey->GetValue();
}
void USP_BlackboardComponent::SetFloat(const FName& EntryName, float Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Float* const FloatKey = Cast<USP_BlackboardKey_Float>(*KeyPtr);
	SP_CHECK(FloatKey, "BoolKey nullptr! Entry with name [%s] is not registered as a float!", *EntryName.ToString())

	FloatKey->SetValue(Value);
}

const FVector& USP_BlackboardComponent::GetVector(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, FVector::ZeroVector, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, FVector::ZeroVector)

	const USP_BlackboardKey_Vector* const VectorKey = Cast<USP_BlackboardKey_Vector>(*KeyPtr);
	SP_RCHECK(VectorKey, FVector::ZeroVector, "BoolKey nullptr! Entry with name [%s] is not registered as a FVector!", *EntryName.ToString())
	
	return VectorKey->GetValue();
}
void USP_BlackboardComponent::SetVector(const FName& EntryName, const FVector& Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Vector* const VectorKey = Cast<USP_BlackboardKey_Vector>(*KeyPtr);
	SP_CHECK(VectorKey, "BoolKey nullptr! Entry with name [%s] is not registered as a FVector!", *EntryName.ToString())

	VectorKey->SetValue(Value);
}

const FRotator& USP_BlackboardComponent::GetRotator(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, FRotator::ZeroRotator, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, FRotator::ZeroRotator)

	const USP_BlackboardKey_Rotator* const RotatorKey = Cast<USP_BlackboardKey_Rotator>(*KeyPtr);
	SP_RCHECK(RotatorKey, FRotator::ZeroRotator, "BoolKey nullptr! Entry with name [%s] is not registered as a FRotator!", *EntryName.ToString())
	
	return RotatorKey->GetValue();
}
void USP_BlackboardComponent::SetRotator(const FName& EntryName, const FRotator& Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Rotator* const RotatorKey = Cast<USP_BlackboardKey_Rotator>(*KeyPtr);
	SP_CHECK(RotatorKey, "BoolKey nullptr! Entry with name [%s] is not registered as a FRotator!", *EntryName.ToString())

	RotatorKey->SetValue(Value);
}

const FName& USP_BlackboardComponent::GetFName(const FName& EntryName) const
{
#if SP_DEBUG
	static const FName NoneName = "None";
#endif

	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, NoneName, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, NoneName)

	const USP_BlackboardKey_Name* const NameKey = Cast<USP_BlackboardKey_Name>(*KeyPtr);
	SP_RCHECK(NameKey, NoneName, "BoolKey nullptr! Entry with name [%s] is not registered as a FName!", *EntryName.ToString())
	
	return NameKey->GetValue();
}
void USP_BlackboardComponent::SetFName(const FName& EntryName, const FName& Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Name* const NameKey = Cast<USP_BlackboardKey_Name>(*KeyPtr);
	SP_CHECK(NameKey, "BoolKey nullptr! Entry with name [%s] is not registered as a FName!", *EntryName.ToString())

	NameKey->SetValue(Value);
}

UObject* USP_BlackboardComponent::GetObject(const FName& EntryName) const
{
	const USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_RCHECK(KeyPtr, nullptr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_RCHECK_NULLPTR(*KeyPtr, nullptr)

	const USP_BlackboardKey_Object* const ObjectKey = Cast<USP_BlackboardKey_Object>(*KeyPtr);
	SP_RCHECK(ObjectKey, nullptr, "BoolKey nullptr! Entry with name [%s] is not registered as an UObject!", *EntryName.ToString())
	
	return ObjectKey->GetValue();
}
void USP_BlackboardComponent::SetObject(const FName& EntryName, UObject* Value)
{
	USP_BlackboardKey* const* const KeyPtr = Keys.Find(EntryName);

	SP_CHECK(KeyPtr, "KeyPtr nullptr! Entry with name [%s] not registered!", *EntryName.ToString())
	SP_CHECK_NULLPTR(*KeyPtr)

	USP_BlackboardKey_Object* const ObjectKey = Cast<USP_BlackboardKey_Object>(*KeyPtr);
	SP_CHECK(ObjectKey, "BoolKey nullptr! Entry with name [%s] is not registered as an UObject!", *EntryName.ToString())

	ObjectKey->SetValue(Value);
}