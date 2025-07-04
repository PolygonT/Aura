// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DefaultAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public: 
    void AbilityActorInfoSet();

    FEffectAssetTags EffectAssetTags;

    void AddCharacterAbilities(
        const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities);

    void AddCharacterAbilitiesNormal(
        const TArray<TSubclassOf<UGameplayAbility>> &NormalAbilities);

    void AbilityInputTagHeld(const FGameplayTag &InputTag);

    void AbilityInputTagReleased(const FGameplayTag &InputTag);

  protected:
    void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
            const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

};
