// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FDefaultGameplayTags {

    static FDefaultGameplayTags &Get();

    static FDefaultGameplayTags* SingletonManager;

    FDefaultGameplayTags();

    TArray<FGameplayTag> Iter;
    TMap<FGameplayTag, FGameplayTag> DamageTypeAndResistanceMap;

    // ----------------------- Attributes -----------------------

    // Vital Attributes
    FGameplayTag Attributes_Vital_Health;
    FGameplayTag Attributes_Vital_Mana;

    // Primary Attributes
    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Resilience;
    FGameplayTag Attributes_Primary_Vigor;

    // Secondary Attributes
    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
    FGameplayTag Attributes_Secondary_MaxHealth;
    FGameplayTag Attributes_Secondary_MaxMana;

    FGameplayTag Attributes_Resistance_Fire;
    FGameplayTag Attributes_Resistance_Lightning;
    FGameplayTag Attributes_Resistance_Arcane;
    FGameplayTag Attributes_Resistance_Physical;

    // ----------------------- Attributes -----------------------

    // ----------------------- Input Tags -----------------------

    FGameplayTag InputTags_RMB;
    FGameplayTag InputTags_LMB;
    FGameplayTag InputTags_1;
    FGameplayTag InputTags_2;
    FGameplayTag InputTags_3;
    FGameplayTag InputTags_4;

    // ----------------------- Input Tags -----------------------

    FGameplayTag Damage;
    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Lightning;
    FGameplayTag Damage_Arcane;
    FGameplayTag Damage_Physical;

    FGameplayTag Effect_HitReact;

    FGameplayTag Ability_Enemy_Melee;
    FGameplayTag Ability_Enemy_Range;
    FGameplayTag Ability_Enemy_Attack;
    FGameplayTag Ability_Pickup_EffectActor;

    FGameplayTag Montage_Attack_Weapon;
    FGameplayTag Montage_Attack_RightHand;
    FGameplayTag Montage_Attack_LeftHand;


    FGameplayTag Cooldown_Fire_FireBolt;

    FGameplayTag GameplayCue_RangeImpact;
    FGameplayTag GameplayCue_Test;
};

