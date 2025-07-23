// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DefaultGameplayTags.h"

UDamageGameplayAbility::UDamageGameplayAbility() {
    for (auto& Pair : FDefaultGameplayTags::Get().DamageTypeAndResistanceMap) {
        FGameplayTag& DamageType = Pair.Key;
        DamageTypesMap.Add(DamageType, {});
    }

    for (const auto& Pair : FDefaultGameplayTags::Get().StackingTypeAndTriggeredMap) {
        StackingTypesMap.Add(Pair.Key, {});
    }
}

void UDamageGameplayAbility::CauseDamage(AActor *TargetActor) {
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

    for (const auto &Pair : DamageTypesMap) {
        const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    }

    for (const auto &Pair : StackingTypesMap) {
        const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    }

    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *SpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}


