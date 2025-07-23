// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DurationalGameplayAbility.h"

void UDurationalGameplayAbility::ApplyCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const {

    UGameplayEffect* CostGE = bStartCost ? 
        StartCostGameplayEffectClass->GetDefaultObject<UGameplayEffect>() :
        GetCostGameplayEffect();

    if (CostGE) {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostGE->GetClass(), GetAbilityLevel());
        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);

        UDurationalGameplayAbility* MutableThis = const_cast<UDurationalGameplayAbility*>(this);
        MutableThis->bStartCost = false;
    }
}

void UDurationalGameplayAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled) {

    bStartCost = true;
    Super::EndAbility(
        Handle,
        ActorInfo,
        ActivationInfo,
        bReplicateEndAbility,
        bWasCancelled
    );
}
