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
        .AddUObject(this, &UOverlayWidgetController::HealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxHealthAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::ManaChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefaultAttributeSet->GetMaxManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

    CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
        [](const FGameplayTagContainer& AssetTags) 
        {
            for (const auto& Tag : AssetTags) {
                GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Black, FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString()));
            }
        }
    );
}


void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) {
    OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) {
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) {
    OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) {
    OnMaxManaChanged.Broadcast(Data.NewValue);
}
