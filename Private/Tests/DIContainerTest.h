#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DIContainerTest.generated.h"

// Test classes for dependency injection testing
UCLASS()
class UTestBaseService : public UObject
{
	GENERATED_BODY()
public:
	virtual FString GetServiceName() const { return TEXT("BaseService"); }
};

UCLASS()
class UTestDerivedService : public UTestBaseService
{
	GENERATED_BODY()
public:
	virtual FString GetServiceName() const override { return TEXT("DerivedService"); }
};

UCLASS()
class UTestAnotherService : public UObject
{
	GENERATED_BODY()
public:
	
	FString GetIdentifier() const { return TEXT("AnotherService"); }
};


UINTERFACE(Blueprintable, BlueprintType)
class UTestAbstraction : public UInterface
{
	GENERATED_BODY()
};

class ITestAbstraction
{
	GENERATED_BODY()

public:

	virtual bool DoSomething() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BP_DoSomething(const int32 Parameter);
};

class ITestCppAbstraction
{
public:
	virtual ~ITestCppAbstraction() = default;
	
	virtual bool DoSomethingNative() = 0;
};

UCLASS()
class UTestAbstractedService : public UObject, public ITestAbstraction, public ITestCppAbstraction
{
	GENERATED_BODY()
public:

	virtual bool DoSomething() override
	{
		UE_LOG(LogTemp, Log, TEXT("Do something called"));
		return true;
	}

	virtual bool DoSomethingNative() override
	{
		UE_LOG(LogTemp, Log, TEXT("Do something native called"));
		return true;
	}
	
	FString GetIdentifier() const { return TEXT("AbstractedService"); }
};
