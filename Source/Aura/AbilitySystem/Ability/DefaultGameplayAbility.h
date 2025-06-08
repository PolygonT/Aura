// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DefaultGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    FGameplayTag AbilityTag;

protected:

};
