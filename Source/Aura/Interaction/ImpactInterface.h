// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ImpactInterface.generated.h"

class USoundBase;
class UNiagaraSystem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UImpactInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IImpactInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    USoundBase* GetImpactSound() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UNiagaraSystem* GetImpactEffectd() const;
};
