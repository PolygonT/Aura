// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultAttributeSet.h"
#include "Net/UnrealNetwork.h"


UDefaultAttributeSet::UDefaultAttributeSet() {
    InitHealth(25.f);
    InitMaxHealth(100.f);
    InitMana(5.f);
    InitMaxMana(50.f);
}

void UDefaultAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UDefaultAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    if (Attribute == GetHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }

    if (Attribute == GetMaxHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0, 10000.f);

    }

    if (Attribute == GetManaAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
    }

    if (Attribute == GetMaxManaAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, 10000.f);
    }
}

void UDefaultAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Health, OldHealth);
}

void UDefaultAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxHealth, OldMaxHealth);
}

void UDefaultAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Mana, OldMana);
}

void UDefaultAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxMana, OldMaxMana);
}
