// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

void UDamageGameplayAbility::CauseDamage(AActor *TargetActor) {
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

    ICombatInterface *CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

    for (const auto& Pair : DamageTypesMap) {
        const float ScaledDamage = Pair.Value.GetValueAtLevel(CombatInterface->GetPlayerLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    }

    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *SpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

