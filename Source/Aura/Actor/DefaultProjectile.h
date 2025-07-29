// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Interaction/EnvDamageInterface.h"
#include "Interaction/ImpactInterface.h"
#include "DefaultProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;
class UGameplayEffect;
class UDefaultGameplayAbility;

UCLASS()
class AURA_API ADefaultProjectile : public AActor, public IImpactInterface, public IEnvDamageInterface
{
	GENERATED_BODY()
	
public:	
	ADefaultProjectile();

    UPROPERTY(EditAnywhere)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY()
    TObjectPtr<UDefaultGameplayAbility> GameplayAbility;

    UPROPERTY()
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypesMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> StackingTypesMap;

    UPROPERTY()
    TObjectPtr<USceneComponent> HomingTarget;

    // ============Impact Interface Start===========
    virtual UNiagaraSystem *GetImpactEffectd_Implementation() const override;
    virtual USoundBase *GetImpactSound_Implementation() const override;
    // ============Impact Interface End=============

    // ============Env Damge Interface Start=============
    virtual TMap<FGameplayTag, FScalableFloat> GetDamageTypesMap() const override;

    virtual TMap<FGameplayTag, FScalableFloat> GetStackingTypesMap() const override;
    // ============Env Damge Interface End===============

protected:

	virtual void BeginPlay() override;

    virtual void Destroyed() override;

    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent,
                         AActor *OtherActor, UPrimitiveComponent *OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep,
                         const FHitResult &SweepResult);

  private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UNiagaraSystem> ImpactEffect;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundBase> ImpactSound;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundBase> LoopingSound;

    bool bHit;

    UPROPERTY(EditAnywhere)
    float LifeSpan {5.f};
};
