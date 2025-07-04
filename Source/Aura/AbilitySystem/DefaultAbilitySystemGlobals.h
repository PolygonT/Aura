// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DefaultAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
    virtual FGameplayEffectContext *AllocGameplayEffectContext() const override;
};
