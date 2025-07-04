// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DefaultProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;
class UGameplayEffect;
class UDefaultGameplayAbility;

UCLASS()
class AURA_API ADefaultProjectile : public AActor
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
