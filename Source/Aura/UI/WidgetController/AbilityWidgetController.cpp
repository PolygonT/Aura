// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AbilityWidgetController.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefaultGameplayTags.h"


void UAbilityWidgetController::BroadcastInitialValues() {
    // ABILITYLIST_SCOPE_LOCK();
    // for (const auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities()) {
    //     FAbilityRow AbilityRow = FAbilityRow {};
    //
    //     UDefaultGameplayAbility *DefaultGameplayAbility = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);
    //
    //     if (!DefaultGameplayAbility) { continue; }
    //
    //     TUniquePtr<FGameplayTag> AbilityTag = nullptr;
    //     for (const auto& AssetTag : DefaultGameplayAbility->GetAssetTags()) {
    //         if (AssetTag.MatchesTag(FDefaultGameplayTags::Get().Ability_Aura)) {
    //             AbilityTag = MakeUnique<FGameplayTag>(AssetTag);
    //             break;
    //         }
    //     }
    //
    //     if (!AbilityTag) { continue; }
    //
    //
    //     AbilityRow.Name = DefaultGameplayAbility->AbilityName;
    //     AbilityRow.Tag = *AbilityTag;
    //     AbilityRow.CooldownTag = DefaultGameplayAbility->GetCooldownTags()->First();
    //     AbilityRow.Level = DefaultGameplayAbility->GetAbilityLevel();
    //
    //     OnAbilityChanged.Broadcast(AbilityRow);
    // }
    
    UDefaultAbilitySystemComponent* DefaultASC = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);
    DefaultASC->BroadcastAllAbilities();
}

void UAbilityWidgetController::BindCallbacksToDependencies() {
    UDefaultAbilitySystemComponent* DefaultASC = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);

    DefaultASC->AbilityInfoDelegate.AddDynamic(this, &ThisClass::OnAbilityChangedCallback);
}

void UAbilityWidgetController::OnAbilityChangedCallback(
    FText Name, FGameplayTag Tag, int32 Level, FGameplayTag CooldownTag) {

    FAbilityRow AbilityRow = {};
    AbilityRow.Name = Name;
    AbilityRow.Tag = Tag;
    AbilityRow.Level = Level;
    AbilityRow.CooldownTag = CooldownTag;

    OnAbilityChanged.Broadcast(AbilityRow);
}

void UAbilityWidgetController::LevelUpAbilityWithTag(FGameplayTag Tag) {
    UDefaultAttributeSet* DefaultAS = CastChecked<UDefaultAttributeSet>(AttributeSet);
    UDefaultAbilitySystemComponent* DefaultASC = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);

    if (!DefaultAS->SpendAttributePoint()) { return; }

    DefaultASC->LevelUpAbilityWithTag(Tag);
}

