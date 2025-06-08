// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Engine/Engine.h"

void UOverlayWidgetController::BroadcastInitialValues() {
    auto DefaultAttributeSet = CastChecked<UDefaultAttributeSet>(AttributeSet);

    OnHealthChanged.Broadcast(DefaultAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(DefaultAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(DefaultAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(DefaultAttributeSet->GetMaxMana());
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

    CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
}
