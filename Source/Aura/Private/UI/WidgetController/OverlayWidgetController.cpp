// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/DefaultAttributeSet.h"

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
