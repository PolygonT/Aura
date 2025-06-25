// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageGameplayAbility : public UDefaultGameplayAbility
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void CauseDamage(AActor *TargetActor);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifiers")
    FScalableFloat AbilityModifierMagnitude;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypesMap;
};
