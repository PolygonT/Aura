// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameplayTagsManager.h"

TArray<FGameplayTag> FDefaultGameplayTags::Iter;

// Vital Attributes
FGameplayTag FDefaultGameplayTags::Attributes_Vital_Health;
FGameplayTag FDefaultGameplayTags::Attributes_Vital_Mana;

// Primary Attributes
FGameplayTag FDefaultGameplayTags::Attributes_Primary_Strength;
FGameplayTag FDefaultGameplayTags::Attributes_Primary_Intelligence;
FGameplayTag FDefaultGameplayTags::Attributes_Primary_Resilience;
FGameplayTag FDefaultGameplayTags::Attributes_Primary_Vigor;

// Secondary Attributes
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_Armor;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_ArmorPenetration;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_BlockChance;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_CriticalHitChance;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_CriticalHitDamage;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_CriticalHitResistance;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_HealthRegeneration;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_ManaRegeneration;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_MaxHealth;
FGameplayTag FDefaultGameplayTags::Attributes_Secondary_MaxMana;

// Input Tags

FGameplayTag FDefaultGameplayTags::InputTags_RMB;
FGameplayTag FDefaultGameplayTags::InputTags_LMB;
FGameplayTag FDefaultGameplayTags::InputTags_1;
FGameplayTag FDefaultGameplayTags::InputTags_2;
FGameplayTag FDefaultGameplayTags::InputTags_3;
FGameplayTag FDefaultGameplayTags::InputTags_4;

void FDefaultGameplayTags::InitNativeGameplayTags() {
    // Vital Attributes
    Attributes_Vital_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Vital.Health");
    Attributes_Vital_Mana =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Vital.Mana");

    // Primary Attributes
    Attributes_Primary_Strength =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Strength", FString("Increases physical damage"));
    Attributes_Primary_Intelligence =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Intelligence", FString("Increases magical damage"));
    Attributes_Primary_Resilience =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Resilience", FString("Increases armor and armor penetration"));
    Attributes_Primary_Vigor =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Vigor", FString("Increases health"));

    // Secondary Attributes
    Attributes_Secondary_Armor =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.Armor", FString("Reduces damage taken, imporoves Block chance"));
    Attributes_Secondary_ArmorPenetration =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ArmorPenetration", FString("Ingore Enemy's Armor"));
    Attributes_Secondary_BlockChance =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.BlockChance", FString("Chance to cut Enemy's in half"));
    Attributes_Secondary_CriticalHitChance =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitChance", FString("Chance to double damage"));
    Attributes_Secondary_CriticalHitDamage =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitDamage", FString("Base critical hit damage"));
    Attributes_Secondary_CriticalHitResistance =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitResistance", FString(""));
    Attributes_Secondary_HealthRegeneration =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.HealthRegeneration", FString(""));
    Attributes_Secondary_ManaRegeneration =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ManaRegeneration", FString(""));
    Attributes_Secondary_MaxHealth =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxHealth", FString(""));
    Attributes_Secondary_MaxMana =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxMana", FString(""));

    // InputTags
    InputTags_RMB  =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.RMB", FString(""));
    InputTags_LMB =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.LMB", FString(""));
    InputTags_1 =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.1", FString(""));
    InputTags_2 =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.2", FString(""));
    InputTags_3 =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.3", FString(""));
    InputTags_4 =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Input.4", FString(""));

    Iter.Add(Attributes_Vital_Health);
    Iter.Add(Attributes_Vital_Mana);
    Iter.Add(Attributes_Primary_Strength);
    Iter.Add(Attributes_Primary_Intelligence);
    Iter.Add(Attributes_Primary_Resilience);
    Iter.Add(Attributes_Primary_Vigor);
    Iter.Add(Attributes_Secondary_Armor);
    Iter.Add(Attributes_Secondary_ArmorPenetration);
    Iter.Add(Attributes_Secondary_BlockChance);
    Iter.Add(Attributes_Secondary_CriticalHitChance);
    Iter.Add(Attributes_Secondary_CriticalHitDamage);
    Iter.Add(Attributes_Secondary_CriticalHitResistance);
    Iter.Add(Attributes_Secondary_HealthRegeneration);
    Iter.Add(Attributes_Secondary_ManaRegeneration);
    Iter.Add(Attributes_Secondary_MaxHealth);
    Iter.Add(Attributes_Secondary_MaxMana);
}
