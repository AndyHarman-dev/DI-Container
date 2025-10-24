// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WithDIContainer.generated.h"

class UDIContainer;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UWithDIContainer : public UInterface
{
	GENERATED_BODY()
};

class DICONTAINERMODULE_API IWithDIContainer
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="DIContainer")
	void SetContainer(const UDIContainer* InContainer);
};
