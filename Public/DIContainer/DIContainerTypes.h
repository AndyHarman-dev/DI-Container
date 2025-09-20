#pragma once
#include "CoreMinimal.h"
#include "DIContainerTypes.generated.h"

#define DEFAULT_HASHABLE_NAME TEXT("Default")

/**
 * A lightweight struct used as a key in hash maps, designed to uniquely
 * identify objects based on their class and an optional FName identifier.
 */
USTRUCT()
struct FHashableClassAndNameKey
{
	GENERATED_BODY()

	FHashableClassAndNameKey() = default;
	FHashableClassAndNameKey(UClass* InClass) : Class(InClass), Name(DEFAULT_HASHABLE_NAME) {}
	FHashableClassAndNameKey(UClass* InClass, FName InName) : Class(InClass), Name(InName) {}

	UPROPERTY()
	UClass* Class;

	UPROPERTY()
	FName Name;

	bool operator==(const FHashableClassAndNameKey& Other) const
	{
		return Class == Other.Class && Name == Other.Name;
	}
};

inline uint32 GetTypeHash(const FHashableClassAndNameKey& Key)
{
	return HashCombine(GetTypeHash(Key.Class), GetTypeHash(Key.Name));
}
