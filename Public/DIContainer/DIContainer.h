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
	 * @param Key - Key with class and an optional name.
	 * @param InInstance Instance to register
	 */
	UFUNCTION(BlueprintCallable, Category="DIContainer")
	void Register(FHashableClassAndNameKey Key, UObject* InInstance);

	/**
	 * Remove references to this specific instance for each class it was registered to
	 * @param InInstance Instance to remove references to
	 */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Unregister"), Category="DIContainer")
	void UnregisterByInstance(UObject* InInstance);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Unregister"), Category="DIContainer")
	void UnregisterByKey(FHashableClassAndNameKey Key);
 
	/**
	 * Resolve instance for BaseType from common container
	 * @return resolved instance
	 */
	UFUNCTION(BlueprintCallable, Category="DIContainer")
	UObject* Resolve(FHashableClassAndNameKey Key);


	/**
	 * Register instance of UObject as a BaseType in container
	 * @param InInstance Instance to register
	 */
	template<UObjectDerived BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(FHashableClassAndNameKey(BaseType::StaticClass()), InInstance);
	}

	template<Interface BaseType>
	void Register(const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(FHashableClassAndNameKey(BaseType::UClassType::StaticClass()), InInstance);
	}
	
	/**
	 * Register instance of UObject as a BaseType in container with unique id
	 * @param OptionalName - Optional name
	 * @param InInstance Instance to register
	 * @param InUniqueId - unique id for the instance
	 */
	template<UObjectDerived BaseType>
	void Register(const TOptional<FName> OptionalName, const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(FHashableClassAndNameKey(BaseType::StaticClass(), OptionalName.Get("")), InInstance);
	}

	template<Interface BaseType>
	void Register(const TOptional<FName> OptionalName, const DerivedFrom<BaseType> auto& InInstance)
	{
		RegisterInternal(FHashableClassAndNameKey(BaseType::UClassType::StaticClass(), OptionalName.Get("")), InInstance);
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
	BaseType* Resolve(const TOptional<FName> OptionalName)
	{
		return Cast<BaseType>(ResolveInternal(FHashableClassAndNameKey(BaseType::StaticClass(), OptionalName.Get(""))));
	}

	template<Interface BaseType>
	BaseType* Resolve(const TOptional<FName> OptionalName)
	{
		return Cast<BaseType>(ResolveInternal(FHashableClassAndNameKey(BaseType::UClassType::StaticClass(), OptionalName.Get(""))));
	}

private:
	
	UPROPERTY()
	TMap<FHashableClassAndNameKey, UObject*> Instances;

	void RegisterInternal(FHashableClassAndNameKey&& Key, UObject *InInstance);
	UObject* ResolveInternal(const FHashableClassAndNameKey& Key);
	bool IsInstanceChildOfOrImplementsInterface(UObject* InInstance, UClass* Class);
};
