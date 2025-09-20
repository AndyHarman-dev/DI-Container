#pragma once
#include "CoreMinimal.h"
#include "DIContainerTypes.generated.h"

/**
 * A lightweight struct used as a key in hash maps, designed to uniquely
 * identify objects based on their class and an optional FName identifier.
 */
USTRUCT(Blueprintable)
struct FHashableClassAndNameKey
{
	GENERATED_BODY()

	FHashableClassAndNameKey() = default;
	FHashableClassAndNameKey(UClass* InClass) : Class(InClass) {}
	FHashableClassAndNameKey(UClass* InClass, const FName& InName) : Class(InClass), OptionalName(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DIContainer")
	UClass* Class = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DIContainer")
	FName OptionalName = TEXT("");

	bool operator==(const FHashableClassAndNameKey& Other) const
	{
		return Class == Other.Class && OptionalName == Other.OptionalName;
	}

	bool IsValid() const noexcept
	{
		return ::IsValid(Class);
	}
};

inline uint32 GetTypeHash(const FHashableClassAndNameKey& Key)
{
	return HashCombine(GetTypeHash(Key.Class), GetTypeHash(Key.OptionalName));
}
