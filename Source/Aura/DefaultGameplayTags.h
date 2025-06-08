// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FDefaultGameplayTags {

    static void InitNativeGameplayTags();

    static TArray<FGameplayTag> Iter;

    // ----------------------- Attributes -----------------------

    // Vital Attributes
    static FGameplayTag Attributes_Vital_Health;
    static FGameplayTag Attributes_Vital_Mana;

    // Primary Attributes
    static FGameplayTag Attributes_Primary_Strength;
    static FGameplayTag Attributes_Primary_Intelligence;
    static FGameplayTag Attributes_Primary_Resilience;
    static FGameplayTag Attributes_Primary_Vigor;

    // Secondary Attributes
    static FGameplayTag Attributes_Secondary_Armor;
    static FGameplayTag Attributes_Secondary_ArmorPenetration;
    static FGameplayTag Attributes_Secondary_BlockChance;
    static FGameplayTag Attributes_Secondary_CriticalHitChance;
    static FGameplayTag Attributes_Secondary_CriticalHitDamage;
    static FGameplayTag Attributes_Secondary_CriticalHitResistance;
    static FGameplayTag Attributes_Secondary_HealthRegeneration;
    static FGameplayTag Attributes_Secondary_ManaRegeneration;
    static FGameplayTag Attributes_Secondary_MaxHealth;
    static FGameplayTag Attributes_Secondary_MaxMana;

    // ----------------------- Attributes -----------------------

    // ----------------------- Input Tags -----------------------

    static FGameplayTag InputTags_RMB;
    static FGameplayTag InputTags_LMB;
    static FGameplayTag InputTags_1;
    static FGameplayTag InputTags_2;
    static FGameplayTag InputTags_3;
    static FGameplayTag InputTags_4;

    // ----------------------- Input Tags -----------------------

};

