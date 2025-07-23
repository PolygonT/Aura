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
  UDamageGameplayAbility();

  UFUNCTION(BlueprintCallable)
  void CauseDamage(AActor *TargetActor);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifiers")
    FScalableFloat AbilityModifierMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypesMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> StackingTypesMap;
};
