// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "DefaultGameplayTags.h"
#include "GameplayEffect.h"
#include "Misc/ObjectThumbnail.h"

const FGameplayTagContainer* UDefaultGameplayAbility::GetCooldownTags() const {
    FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
    MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
    const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();

    if (ParentTags) {
        MutableTags->AppendTags(*ParentTags);
    }

    MutableTags->AppendTags(CooldownTags);

    return MutableTags;
}

void UDefaultGameplayAbility::ApplyCooldown(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const {
    UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();

    if (CooldownGE) {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
        SpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
        SpecHandle.Data->SetSetByCallerMagnitude(
            FDefaultGameplayTags::Get().SetByCaller_GA_Cooldown,
            CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
    }
}


