// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAbilitySystemComponent.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "DefaultGameplayTags.h"
#include "Player/DefaultPlayerState.h"

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
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(DefaultGameplayAbility->AbilityTag);
            GiveAbility(AbilitySpec);
        }
    }
}

void UDefaultAbilitySystemComponent::AddCharacterAbilitiesNormal(
    const TArray<TSubclassOf<UGameplayAbility>> &NormalAbilities) {
    for (auto& ItemClass : NormalAbilities) {
        auto AbilitySpec = FGameplayAbilitySpec(ItemClass, 1);
        GiveAbility(AbilitySpec);
    }
}

void UDefaultAbilitySystemComponent::AbilityInputTagHeld(
    const FGameplayTag &InputTag) {
    if (!InputTag.IsValid()) {
        return;
    }

    // TODO 3: remove this for loop
    for (auto& AbilitySpec : GetActivatableAbilities()) {
        if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) {
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
        if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) {
            AbilitySpecInputReleased(AbilitySpec);

            if (AbilitySpec.IsActive()) {
                UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance();
                if (PrimaryInstance) {
                    InvokeReplicatedEvent(
                        EAbilityGenericReplicatedEvent::InputReleased,
                        AbilitySpec.Handle,
                        PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
                }
            }
        }
    }
}

void UDefaultAbilitySystemComponent::LevelUpAbilityWithTag(FGameplayTag Tag) {
    ABILITYLIST_SCOPE_LOCK();
    for (auto& AbilitySpec : ActivatableAbilities.Items) {
        if (AbilitySpecHasTag(AbilitySpec, Tag, true)) {
            LevelUpAbilityInternal(AbilitySpec);

            return;
        }
    }
}

void UDefaultAbilitySystemComponent::LevelUpAbility(TSubclassOf<UGameplayAbility> AbilityClass) {
    FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
    if (!AbilitySpec) { return; }

    LevelUpAbilityInternal(*AbilitySpec);
}


void UDefaultAbilitySystemComponent::BroadcastAllAbilities() {
    ABILITYLIST_SCOPE_LOCK();
    for (const auto& AbilitySpec : ActivatableAbilities.Items) {
        UDefaultGameplayAbility *DefaultGameplayAbility = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);

        if (!DefaultGameplayAbility) { continue; }

        const FGameplayTag* AbilityAssetTag = AbilitySpecHasTag(AbilitySpec, FDefaultGameplayTags::Get().Ability_Aura, false);
        if (!AbilityAssetTag) {
            continue;
        }

        AbilityInfoDelegate.Broadcast(
            DefaultGameplayAbility->AbilityName, *AbilityAssetTag,
            AbilitySpec.Level,
            DefaultGameplayAbility->CooldownTags.First());
    }
}

FOnAbilityLevelChangeSignature&
UDefaultAbilitySystemComponent::GetOnLevelChangeDelegate() {
    return AbilityInfoDelegate;
}

const FGameplayTag* UDefaultAbilitySystemComponent::AbilitySpecHasTag(const FGameplayAbilitySpec& AbilitySpec, FGameplayTag Tag, bool ExactMatch) {
    UDefaultGameplayAbility* DefaultGA = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);
    if (!DefaultGA) {
        return nullptr;
    }

    const FGameplayTag* AbilityAssetTag = nullptr;

    for (const auto& AssetTag : DefaultGA->GetAssetTags()) {
        if (AssetTag.MatchesTag(FDefaultGameplayTags::Get().Ability_Aura)) {
            AbilityAssetTag = &AssetTag;
            break;
        }
    }

    if (!AbilityAssetTag) {
        return nullptr;
    }

    if (ExactMatch) {
        return AbilityAssetTag->MatchesTagExact(Tag) ? AbilityAssetTag : nullptr;
    } else {
        return AbilityAssetTag->MatchesTag(Tag) ? AbilityAssetTag : nullptr;
    }
}

void UDefaultAbilitySystemComponent::LevelUpAbilityInternal(
    FGameplayAbilitySpec &AbilitySpec) {

    AbilitySpec.Level += 1;

    MarkAbilitySpecDirty(AbilitySpec);

    
    UDefaultGameplayAbility* DefaultGA = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);

    const FGameplayTag* AbilityAssetTag = AbilitySpecHasTag(AbilitySpec, FDefaultGameplayTags::Get().Ability_Aura, false);

    if (!AbilityAssetTag) {
        return;
    }
    
    AbilityInfoDelegate.Broadcast(DefaultGA->AbilityName, *AbilityAssetTag, AbilitySpec.Level, DefaultGA->CooldownTags.First());
}

