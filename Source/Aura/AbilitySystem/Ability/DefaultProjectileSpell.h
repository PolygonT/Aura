// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "DefaultProjectileSpell.generated.h"

class ADefaultProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UDefaultProjectileSpell : public UDamageGameplayAbility
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultProjectile> ProjectileClass;



    // virtual void
    // ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    //                 const FGameplayAbilityActorInfo *ActorInfo,
    //                 const FGameplayAbilityActivationInfo ActivationInfo,
    //                 const FGameplayEventData *TriggerEventData) override;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectile(const FVector& ProjectileTargetLocation);
};
