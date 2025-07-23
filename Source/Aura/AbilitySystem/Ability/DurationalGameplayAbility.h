// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "DurationalGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDurationalGameplayAbility : public UDefaultGameplayAbility
{
	GENERATED_BODY()

public:
    virtual void
    ApplyCost(const FGameplayAbilitySpecHandle Handle,
              const FGameplayAbilityActorInfo *ActorInfo,
              const FGameplayAbilityActivationInfo ActivationInfo) const override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo *ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility,
                            bool bWasCancelled) override;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Costs")
    TSubclassOf<UGameplayEffect> StartCostGameplayEffectClass; 

private:
    // 第一次Cost用GetCostEffect,每个DeltaTime用PeriodicCostGameplayEffect
    bool bStartCost { true };
};
