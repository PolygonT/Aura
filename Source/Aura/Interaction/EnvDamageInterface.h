// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "UObject/Interface.h"
#include "EnvDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnvDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnvDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual TMap<FGameplayTag, FScalableFloat> GetDamageTypesMap() const = 0;

    virtual TMap<FGameplayTag, FScalableFloat> GetStackingTypesMap() const = 0;

};
