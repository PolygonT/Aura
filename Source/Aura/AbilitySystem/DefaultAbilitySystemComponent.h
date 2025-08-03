// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DefaultAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAbilityLevelChangeSignature, FText, Name, FGameplayTag, Tag, int32, Level, FGameplayTag, CooldownTag);

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
        const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities, int32 CharacterLevel);

    void AddCharacterAbilitiesNormal(
        const TArray<TSubclassOf<UGameplayAbility>> &NormalAbilities, int32 CharacterLevel);

    void AbilityInputTagHeld(const FGameplayTag &InputTag);

    void AbilityInputTagReleased(const FGameplayTag &InputTag);

    FOnAbilityLevelChangeSignature& GetOnLevelChangeDelegate();

    UFUNCTION(BlueprintCallable)
    void LevelUpAbilityWithTag(FGameplayTag Tag);

    UFUNCTION(BlueprintCallable)
    void LevelUpAbility(TSubclassOf<UGameplayAbility> AbilityClass);

    void BroadcastAllAbilities();

    UPROPERTY()
    FOnAbilityLevelChangeSignature AbilityInfoDelegate;

protected:
    

    void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
            const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
private:

    const FGameplayTag* AbilitySpecHasTag(const FGameplayAbilitySpec& AbilitySpec, FGameplayTag Tag, bool ExactMatch);

    void LevelUpAbilityInternal(FGameplayAbilitySpec &AbilitySpec);
};
