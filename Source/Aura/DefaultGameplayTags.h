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
    TMap<FGameplayTag, FGameplayTag> DamageTypeAndStackingTriggeredMap;
    TMap<FGameplayTag, FGameplayTag> StackingTypeAndTriggeredMap;

    // ----------------------- Attributes -----------------------

    // Vital Attributes
    FGameplayTag Attributes_Vital_Health;
    FGameplayTag Attributes_Vital_Mana;
    FGameplayTag Attributes_Vital_FireStacking;
    FGameplayTag Attributes_Vital_LightningStacking;

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
    FGameplayTag Attributes_Secondary_MaxFireStacking;
    FGameplayTag Attributes_Secondary_MaxLightningStacking;

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

    FGameplayTag Stacking_Fire;
    FGameplayTag Stacking_Fire_Triggered;
    FGameplayTag Stacking_Lightning;
    FGameplayTag Stacking_Lightning_Triggered;

    FGameplayTag Effect;
    FGameplayTag Effect_HitReact;
    FGameplayTag Effect_EnvDamage;
    FGameplayTag Effect_StackingDamage;
    FGameplayTag Effect_Stun;

    FGameplayTag Ability_Enemy_Melee;
    FGameplayTag Ability_Enemy_Range;
    FGameplayTag Ability_Enemy_Attack;
    FGameplayTag Ability_Pickup_EffectActor;
    FGameplayTag Ability_Aura;
    FGameplayTag Ability_Aura_FireBolt;
    FGameplayTag Ability_Aura_LightningShock;

    FGameplayTag PunishAbility_HitReact;
    FGameplayTag PunishAbility_Stun;

    FGameplayTag Montage_Attack_Weapon;
    FGameplayTag Montage_Attack_RightHand;
    FGameplayTag Montage_Attack_LeftHand;


    FGameplayTag Cooldown_Fire_FireBolt;
    FGameplayTag Cooldown_LightningShock;

    FGameplayTag GameplayCue_RangeImpact;
    FGameplayTag GameplayCue_Test;

    // ----------------------- Set By Caller Tags ---------------------------

    FGameplayTag SetByCaller_GA_Cooldown;
    FGameplayTag SetByCaller_GE_GainXp;

    // ----------------------- Set By Caller Tags End -----------------------

};

