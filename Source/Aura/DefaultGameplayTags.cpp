// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameplayTagsManager.h"
#include "UObject/ObjectPtr.h"

FDefaultGameplayTags* FDefaultGameplayTags::SingletonManager;

FDefaultGameplayTags::FDefaultGameplayTags() {
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

    Attributes_Resistance_Fire =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Fire", FString(""));
    Attributes_Resistance_Lightning =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Lightning", FString(""));
    Attributes_Resistance_Arcane =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Arcane", FString(""));
    Attributes_Resistance_Physical =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Resistance.Physical", FString(""));

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

    Damage =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Damge", FString(""));
    Damage_Fire =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Damge.Fire", FString(""));
    Damage_Lightning =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Damge.Lightning", FString(""));
    Damage_Arcane =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Damge.Arcane", FString(""));
    Damage_Physical =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Damge.Physical", FString(""));

    Effect_HitReact =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Effects.HitReact", FString(""));

    Ability_Enemy_Melee =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Ability.Enemy.Melee", FString(""));

    Montage_Attack_Weapon =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Montage.Attack.Weapon", FString(""));
    Montage_Attack_RightHand =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Montage.Attack.RightHand", FString(""));
    Montage_Attack_LeftHand =
        UGameplayTagsManager::Get().AddNativeGameplayTag("Montage.Attack.LeftHand", FString(""));

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
    Iter.Add(Attributes_Resistance_Fire);
    Iter.Add(Attributes_Resistance_Lightning);
    Iter.Add(Attributes_Resistance_Arcane);
    Iter.Add(Attributes_Resistance_Physical);

    DamageTypeAndResistanceMap.Add(Damage_Fire, Attributes_Resistance_Fire);
    DamageTypeAndResistanceMap.Add(Damage_Lightning, Attributes_Resistance_Lightning);
    DamageTypeAndResistanceMap.Add(Damage_Arcane, Attributes_Resistance_Arcane);
    DamageTypeAndResistanceMap.Add(Damage_Physical, Attributes_Resistance_Physical);
}

FDefaultGameplayTags &FDefaultGameplayTags::Get() {
    if (!SingletonManager) {
        // TODO 4 How to remove this new key
        SingletonManager = new FDefaultGameplayTags();
    }

    return *SingletonManager;
}

