// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAttributeSet.h"
#include "DefaultGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


UDefaultAttributeSet::UDefaultAttributeSet() {
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Vital_Health, UDefaultAttributeSet::GetHealthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Vital_Mana, UDefaultAttributeSet::GetManaAttribute);

    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Primary_Strength, UDefaultAttributeSet::GetStrengthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Primary_Resilience, UDefaultAttributeSet::GetResilienceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Primary_Intelligence, UDefaultAttributeSet::GetIntelligenceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Primary_Vigor, UDefaultAttributeSet::GetVigorAttribute);

    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_Armor                , UDefaultAttributeSet::GetArmorAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_ArmorPenetration     , UDefaultAttributeSet::GetArmorPenetrationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_BlockChance          , UDefaultAttributeSet::GetBlockChanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_CriticalHitChance    , UDefaultAttributeSet::GetCriticalHitChanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_CriticalHitDamage    , UDefaultAttributeSet::GetCriticalHitDamageAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_CriticalHitResistance, UDefaultAttributeSet::GetCriticalHitResistanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_HealthRegeneration   , UDefaultAttributeSet::GetHealthRegenerationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_ManaRegeneration     , UDefaultAttributeSet::GetManaRegenerationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_MaxHealth            , UDefaultAttributeSet::GetMaxHealthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Attributes_Secondary_MaxMana              , UDefaultAttributeSet::GetMaxManaAttribute);
}

void UDefaultAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UDefaultAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    if (Attribute == GetHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }

    if (Attribute == GetManaAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
    }

    Super::PreAttributeChange(Attribute, NewValue);
}

void UDefaultAttributeSet::PostGameplayEffectExecute(
    const FGameplayEffectModCallbackData &Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
        SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
    }

    if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
        SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
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

void UDefaultAttributeSet::OnRep_Strength(const FGameplayAttributeData OldStrength) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Strength, OldStrength);
}

void UDefaultAttributeSet::OnRep_Intelligence(const FGameplayAttributeData OldIntelligence) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Intelligence, OldIntelligence);
}

void UDefaultAttributeSet::OnRep_Resilience(const FGameplayAttributeData OldResilience) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Resilience, OldResilience);
}

void UDefaultAttributeSet::OnRep_Vigor(const FGameplayAttributeData OldVigor) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Vigor, OldVigor);
}

void UDefaultAttributeSet::OnRep_Armor(const FGameplayAttributeData OldArmor) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Armor, OldArmor);
}

void UDefaultAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UDefaultAttributeSet::OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, BlockChance, OldBlockChance);
}

void UDefaultAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UDefaultAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UDefaultAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}


void UDefaultAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UDefaultAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData OldManaRegeneration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ManaRegeneration, OldManaRegeneration);
}
