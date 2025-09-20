// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DIContainerTypes.h"
#include "DIContainer.generated.h"


template<typename T>
concept UObjectDerived = std::is_base_of_v<UObject, T>;

template<typename Derived, typename Base>
concept DerivedFrom = TIsDerivedFrom<typename TRemovePointer<Derived>::Type, Base>::Value;

template<typename T>
concept Interface = static_cast<bool>(TIsIInterface<T>::Value);


/**
 * Dependency Injection container for registering and resolving UObject instances.
 * Instances can be registered by type and later resolved for dependency injection purposes.
 */
UCLASS(NotBlueprintable, BlueprintType)
class DICONTAINERMODULE_API UDIContainer : public UObject
{
	GENERATED_BODY()

public:
	
	/**
	 * Register instance of UObject as a BaseType in common container
	 * @param InInstance Instance to register
	 */
	UFUNCTION(BlueprintCallable)
	void Register(UObject* InInstance, TSubclassOf<UObject> InClass);

	/**
	 * Register instance of UObject as a BaseType in common container with unique id
	 * @param InInstance - Instance to register
	 * @param InClass - class of the instance
	 * @param InUniqueId - unique id for the instance
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterWithNameId(UObject* InInstance, TSubclassOf<UObject> InClass, const FName& InUniqueId);

	/**
	 * Remove references to this specific instance for each class it was registered to
	 * @param InInstance Instance to remove references to
	 */
	UFUNCTION(BlueprintCallable)
	void Unregister(UObject* InInstance);
 
	/**
	 * Resolve instance for BaseType from common container
	 * @return resolved instance
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="InClass"))
	UObject* Resolve(TSubclassOf<UObject> InClass);

	/**
	 * Resolve instance for BaseType from common container with unique id
	 * @return resolved instance
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="InClass"))
	UObject* ResolveWithNameId(TSubclassOf<UObject> InClass, const FName& InUniqueId);


	/**
	 * Register instance of UObject as a BaseType in container
	 * @param InInstance Instance to register
	 */
	template<UObjectDerived BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(InInstance, BaseType::StaticClass());
	}

	template<Interface BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(InInstance, BaseType::UClassType::StaticClass());
	}
	
	/**
	 * Register instance of UObject as a BaseType in container with unique id
	 * @param InInstance Instance to register
	 * @param InUniqueId - unique id for the instance
	 */
	template<UObjectDerived BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance, const FName& InUniqueId)
	{
		RegisterInternal(InInstance, BaseType::StaticClass(), InUniqueId);
	}

	template<Interface BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance, const FName& InUniqueId)
	{
		RegisterInternal(InInstance, BaseType::UClassType::StaticClass(), InUniqueId);
	}
	
	/**
	 * Resolve instance for BaseType
	 * @return resolved instance
	 */
	template<UObjectDerived BaseType>
	BaseType* Resolve()
	{
		return Cast<BaseType>(ResolveInternal(BaseType::StaticClass()));
	}

	template<Interface BaseType>
	BaseType* Resolve()
	{
		return Cast<BaseType>(ResolveInternal(BaseType::UClassType::StaticClass()));
	}

	/**
	 * Resolve instance for BaseType
	 * @return resolved instance
	 */
	template<UObjectDerived BaseType>
	BaseType* Resolve(const FName& InUniqueId)
	{
		return Cast<BaseType>(ResolveInternal(BaseType::StaticClass(), InUniqueId));
	}

	template<Interface BaseType>
	BaseType* Resolve(const FName& InUniqueId)
	{
		return Cast<BaseType>(ResolveInternal(BaseType::UClassType::StaticClass(), InUniqueId));
	}

private:
	
	UPROPERTY()
	TMap<FHashableClassAndNameKey, UObject*> Instances;

	void RegisterInternal(UObject *InInstance, UClass *InClass, const FName& InUniqueId = DEFAULT_HASHABLE_NAME);
	UObject* ResolveInternal(UClass* InClass, const FName& InUniqueId = DEFAULT_HASHABLE_NAME);
};
