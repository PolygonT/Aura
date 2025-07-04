// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Controller.h"
#include "DefaultAttributeSet.generated.h"

class ACharacter;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties {
    GENERATED_BODY()

    UPROPERTY()
    FGameplayEffectContextHandle EffectContextHandle;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<AActor> SourceAvatarActor;

    UPROPERTY()
    TObjectPtr<AController> SourceController;

    UPROPERTY()
    TObjectPtr<ACharacter> SourceCharacter;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<AActor> TargetAvatarActor;

    UPROPERTY()
    TObjectPtr<AController> TargetController;

    UPROPERTY()
    TObjectPtr<ACharacter> TargetCharacter;
};

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    UDefaultAttributeSet();

    //~ Start Member viriables

    /**
    * Primary Attributes
    */

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Strength)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Intelligence)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
    FGameplayAttributeData Resilience;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Resilience)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
    FGameplayAttributeData Vigor;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Vigor)

    /**
    * Secondary Attributes
    */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Armor)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
    FGameplayAttributeData ArmorPenetration;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ArmorPenetration)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
    FGameplayAttributeData BlockChance;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, BlockChance)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitChance;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, CriticalHitChance)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitDamage;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, CriticalHitDamage)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitResistance;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, CriticalHitResistance)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
    FGameplayAttributeData HealthRegeneration;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, HealthRegeneration)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
    FGameplayAttributeData ManaRegeneration;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ManaRegeneration)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, MaxMana)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceFire, Category = "Secondary Attributes")
    FGameplayAttributeData ResistanceFire;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ResistanceFire)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceLightning, Category = "Secondary Attributes")
    FGameplayAttributeData ResistanceLightning;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ResistanceLightning)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceArcane, Category = "Secondary Attributes")
    FGameplayAttributeData ResistanceArcane;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ResistanceArcane)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistancePhysical, Category = "Secondary Attributes")
    FGameplayAttributeData ResistancePhysical;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, ResistancePhysical)

    /**
    * Vital Attributes
    */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Mana)

    /**
    * Meta Attributes (Does't Replicate)
    */
    UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
    FGameplayAttributeData IncomingDamage;
    ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, IncomingDamage)
    
    TMap<FGameplayTag, FGameplayAttribute(*)()> AttributeGetMap;

    //~ End Member viriables

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;

    virtual void PostGameplayEffectExecute(
        const FGameplayEffectModCallbackData &Data) override;

    //~ Start Member function
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData OldHealth) const;

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData OldMana) const;

    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData OldStrength) const;

    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData OldIntelligence) const;

    UFUNCTION()
    void OnRep_Resilience(const FGameplayAttributeData OldResilience) const;

    UFUNCTION()
    void OnRep_Vigor(const FGameplayAttributeData OldVigor) const;

    UFUNCTION()
    void OnRep_Armor(const FGameplayAttributeData OldArmor) const;

    UFUNCTION()
    void OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const;
    
    UFUNCTION()
    void OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const;

    UFUNCTION()
    void OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const;

    UFUNCTION()
    void OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const;
    
    UFUNCTION()
    void OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const;

    UFUNCTION()
    void OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const;

    UFUNCTION()
    void OnRep_ManaRegeneration(const FGameplayAttributeData OldManaRegeneration) const;

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const;

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const;

    UFUNCTION()
    void OnRep_ResistanceFire(const FGameplayAttributeData OldResistanceFire) const;

    UFUNCTION()
    void OnRep_ResistanceLightning(const FGameplayAttributeData OldResistanceLightning) const;

    UFUNCTION()
    void OnRep_ResistanceArcane(const FGameplayAttributeData OldResistanceArcane) const;

    UFUNCTION()
    void OnRep_ResistancePhysical(const FGameplayAttributeData OldResistancePhysical) const;
    //~ End Member function
private:
  void SetEffectProperties(const FGameplayEffectModCallbackData &Data,
                           FEffectProperties& Props);
};
