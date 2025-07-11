// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ScalableFloat.h"
#include "DefaultGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    FGameplayTag AbilityTag;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
    FScalableFloat CooldownDuration;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
    FGameplayTagContainer CooldownTags;


    virtual const FGameplayTagContainer *GetCooldownTags() const override;

    virtual void ApplyCooldown(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) const override;

  protected:
  private:
    // Temp container that we will return the pointer to in GetCooldownTags().
    // This will be a union of our CooldownTags and the Cooldown GE's cooldown
    // tags.
    UPROPERTY(Transient)
    FGameplayTagContainer TempCooldownTags;
};
