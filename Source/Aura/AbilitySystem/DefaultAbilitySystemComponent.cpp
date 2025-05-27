// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAbilitySystemComponent.h"

void UDefaultAbilitySystemComponent::AbilityActorInfoSet() {
    this->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDefaultAbilitySystemComponent::EffectApplied);
}

void UDefaultAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
            const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
    FGameplayTagContainer TagContainer {};
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}

