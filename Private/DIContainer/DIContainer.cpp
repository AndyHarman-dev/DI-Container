// Fill out your copyright notice in the Description page of Project Settings.


#include "DIContainer/DIContainer.h"

DEFINE_LOG_CATEGORY_STATIC(LogDIContainer, Log, All);

void UDIContainer::Register(UObject* InInstance, const TSubclassOf<UObject> InClass)
{
	if (!IsValid(InInstance) || !IsValid(InClass))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: InInstance or InClass is not valid"));
		return;
	}

	const bool bImplementsInterfaceOrIsA =
		InClass->HasAnyClassFlags(CLASS_Interface) ?
	InInstance->GetClass()->ImplementsInterface(InClass) :
	InInstance->IsA(InClass);
	
	if (!bImplementsInterfaceOrIsA)
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: Instance is not implemented base type %s"), *(InClass->GetName()));
		return;
	}

	RegisterInternal(InInstance, InClass);
}

void UDIContainer::RegisterWithNameId(UObject* InInstance, TSubclassOf<UObject> InClass, const FName& InUniqueId)
{
	if (!IsValid(InInstance) || !IsValid(InClass))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: InInstance or InClass is not valid"));
		return;
	}

	if (!InInstance->IsA(InClass))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: Instance is not implemented base type %s"), *(InClass->GetName()));
		return;
	}

	RegisterInternal(InInstance, InClass, InUniqueId);
}

void UDIContainer::Unregister(UObject* InInstance)
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

UObject* UDIContainer::Resolve(const TSubclassOf<UObject> InClass)
{
	if (!IsValid(InClass))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Resolve: InClass is not valid"));
		return nullptr;
	}
 
	return ResolveInternal(InClass);
}

UObject* UDIContainer::ResolveWithNameId(TSubclassOf<UObject> InClass, const FName& InUniqueId)
{
	if (!IsValid(InClass))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Resolve: InClass is not valid"));
		return nullptr;
	}
 
	return ResolveInternal(InClass, InUniqueId);
}

void UDIContainer::RegisterInternal(UObject* InInstance, UClass* InClass, const FName& InUniqueId)
{
	if (!IsValid(InInstance))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: Instance is not valid when attempt to register %s"), *(InClass->GetName()));
		return;
	}

	if (Instances.Contains(FHashableClassAndNameKey(InClass, InUniqueId)))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Register: Instance of type %s is already registerd"), *(InClass->GetName()));
		return;
	}

	Instances.Add(FHashableClassAndNameKey(InClass, InUniqueId), InInstance);
}

UObject* UDIContainer::ResolveInternal(UClass* InClass, const FName& InUniqueId)
{
	FHashableClassAndNameKey Key(InClass, InUniqueId);
	if (!Instances.Contains(Key))
	{
		UE_LOG(LogDIContainer, Error, TEXT("Resolve: Instance of type %s is not registerd"), *(InClass->GetName()));
		return nullptr;
	}

	return Instances[Key];
}