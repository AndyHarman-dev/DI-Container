// Fill out your copyright notice in the Description page of Project Settings.


#include "DIContainer/DIContainer.h"

#include "UELoggingModule/Public/ValidateMacro.h"

DEFINE_LOG_CATEGORY_STATIC(LogDIContainer, Log, All);

void UDIContainer::Register(FHashableClassAndNameKey Key, UObject* InInstance)
{
	if (!ValidateMsfg(Key.IsValid() && IsValid(InInstance), LogDIContainer, "InInstance or InClass is not valid"))
	{
		return;
	}

	if (!ValidateMsfg(IsInstanceChildOfOrImplementsInterface(InInstance, Key.Class), LogDIContainer, "Instance is not implemented base type %s", *Key.Class->GetName()))
	{
		return;
	}

	RegisterInternal(MoveTemp(Key), InInstance);
}

void UDIContainer::UnregisterByInstance(UObject* InInstance)
{
	for (auto Iter = Instances.CreateIterator(); Iter; ++Iter)
	{
		auto LInstance = Iter->Value;
		if (LInstance == InInstance)
		{
			Iter.RemoveCurrent();
		}
	}
}

void UDIContainer::UnregisterByKey(FHashableClassAndNameKey Key)
{
	if (!ValidateMsfg(Instances.Contains(Key), LogDIContainer, "Instance of type %s is not registerd", *(Key.Class->GetName())))
	{
		return;
	}

	Instances.Remove(Key);
}

UObject* UDIContainer::Resolve(FHashableClassAndNameKey Key)
{
	if (!ValidateMsfg(Key.IsValid(), LogDIContainer, "InClass is not valid"))
	{
		return nullptr;
	}
 
	return ResolveInternal(Key);
}

void UDIContainer::RegisterInternal(FHashableClassAndNameKey&& Key, UObject* InInstance)
{
	if (!ValidateMsfg(IsValid(InInstance), LogDIContainer, "InInstance is not valid when attempt to register %s", *(Key.Class->GetName())))
	{
		return;
	}

	if (Instances.Contains(Key))
	{
		UE_LOG(LogDIContainer, Warning, TEXT("Instance of type %s is already registerd"), *(Key.Class->GetName()));
		return;
	}

	Instances.Add(MoveTemp(Key), InInstance);
}

UObject* UDIContainer::ResolveInternal(const FHashableClassAndNameKey& Key)
{
	if (!ValidateMsfg(Instances.Contains(Key), LogDIContainer, "Instance of type %s is not registerd", *(Key.Class->GetName())))
	{
		return nullptr;
	}

	return Instances[Key];
}

bool UDIContainer::IsInstanceChildOfOrImplementsInterface(UObject* InInstance, UClass* Class)
{
	return Class->HasAnyClassFlags(CLASS_Interface) ? InInstance->GetClass()->ImplementsInterface(Class) : InInstance->IsA(Class);
}
