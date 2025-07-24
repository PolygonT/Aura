// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "DefaultGameplayTags.h"
#include "Engine/Engine.h"
#include "GameplayAbilitySpec.h"

void UOverlayWidgetController::BroadcastInitialValues() {
    auto DefaultAttributeSet = CastChecked<UDefaultAttributeSet>(AttributeSet);

    OnHealthChanged.Broadcast(DefaultAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(DefaultAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(DefaultAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(DefaultAttributeSet->GetMaxMana());

    // ABILITYLIST_SCOPE_LOCK();
    for (const auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities()) {
        FAbilityRow AbilityRow = FAbilityRow {};

        UDefaultGameplayAbility *DefaultGameplayAbility = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);

        if (!DefaultGameplayAbility) { continue; }

        TUniquePtr<FGameplayTag> AbilityTag = nullptr;
        for (const auto& AssetTag : DefaultGameplayAbility->GetAssetTags()) {
            if (AssetTag.MatchesTag(FDefaultGameplayTags::Get().Ability_Aura)) {
                AbilityTag = MakeUnique<FGameplayTag>(AssetTag);
                break;
            }
        }

        if (!AbilityTag) { continue; }


        AbilityRow.Name = DefaultGameplayAbility->AbilityName;
        AbilityRow.Tag = *AbilityTag;
        AbilityRow.CooldownTag = DefaultGameplayAbility->GetCooldownTags()->First();
        AbilityRow.Level = DefaultGameplayAbility->GetAbilityLevel();

        OnAbilityChanged.Broadcast(AbilityRow);
    }
}

void UOverlayWidgetController::BindCallbacksToDependencies() {
    auto DefaultAttributeSet = CastChecked<UDefaultAttributeSet>(AttributeSet);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetHealthAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxHealthAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetManaAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxManaAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });

    UDefaultAbilitySystemComponent* DefaultASC = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);
    DefaultASC->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags) 
        {
            for (const auto& Tag : AssetTags) {
                GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Black, FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString()));

                auto MessageTag = FGameplayTag::RequestGameplayTag("Message");
                if (Tag.MatchesTag(MessageTag)) {
                    auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    // Row->MessageTag = Tag;
                    MessageWidgetRowDelegate.Broadcast(*Row);
                }
            }
        }
    );

    // Ability Level Change Delegate
    DefaultASC->LevelChangeDelegate.AddDynamic(this, &ThisClass::OnAbilityChangedCallback);

    // Character Level Change Delegate
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetLevelAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { 
            if (Data.NewValue > 1.f) {
                OnLevelChanged.Broadcast(Data.NewValue); 
            }
        });

    // Character Stacking Value Change Delegate
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetFireStackingAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnFireStackingChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetLightningStackingAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnLightningStackingChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxFireStackingAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnMaxFireStackingChanged.Broadcast(Data.NewValue); });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxLightningStackingAttribute())
        .AddLambda([this] (const FOnAttributeChangeData& Data) { OnMaxLightningStackingChanged.Broadcast(Data.NewValue); });
}

void UOverlayWidgetController::OnAbilityChangedCallback(FText Name, FGameplayTag Tag, int32 Level, FGameplayTag CooldownTag) {
    FAbilityRow AbilityRow = {};
    AbilityRow.Name = Name;
    AbilityRow.Tag = Tag;
    AbilityRow.Level = Level;
    AbilityRow.CooldownTag = CooldownTag;

    OnAbilityChanged.Broadcast(AbilityRow);
}

