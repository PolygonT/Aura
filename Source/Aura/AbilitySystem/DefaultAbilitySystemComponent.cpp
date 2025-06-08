// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAbilitySystemComponent.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"

void UDefaultAbilitySystemComponent::AbilityActorInfoSet() {
    this->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDefaultAbilitySystemComponent::EffectApplied);
}

void UDefaultAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
            const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
    FGameplayTagContainer TagContainer {};
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}

void UDefaultAbilitySystemComponent::AddCharacterAbilities(
    const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities) {
    for (auto& ItemClass : StartupAbilities) {
        auto AbilitySpec = FGameplayAbilitySpec(ItemClass, 1);

        if (auto DefaultGameplayAbility = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability)) {
            AbilitySpec.DynamicAbilityTags.AddTag(DefaultGameplayAbility->AbilityTag);
            GiveAbility(AbilitySpec);
        }
    }
}

void UDefaultAbilitySystemComponent::AbilityInputTagHeld(
    const FGameplayTag &InputTag) {
    if (!InputTag.IsValid()) {
        return;
    }

    // TODO 3: remove this for loop
    for (auto& AbilitySpec : GetActivatableAbilities()) {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive()) {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UDefaultAbilitySystemComponent::AbilityInputTagReleased(
    const FGameplayTag &InputTag) {
    if (!InputTag.IsValid()) {
        return;
    }

    for (auto& AbilitySpec : GetActivatableAbilities()) {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

