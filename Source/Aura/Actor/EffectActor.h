// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Interaction/EnvDamageInterface.h"
#include "ScalableFloat.h"
#include "EffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
    DoNotRemove,
    RemoveOnEndOverlap
};

class UGameplayEffect;
struct FGameplayEffectSpecHandle;
class UCapsuleComponent;

UCLASS()
class AURA_API AEffectActor : public AActor, public IEnvDamageInterface
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass);

    UFUNCTION(BlueprintCallable)
    void RemoveActiveEffect(AActor* TargetActor);

    // ==========IEnvDamageInterface==============
    virtual TMap<FGameplayTag, FScalableFloat>
    GetDamageTypesMap() const override;

    virtual TMap<FGameplayTag, FScalableFloat>
    GetStackingTypesMap() const override;

    // ==========IEnvDamageInterface End==========

protected:
	virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCapsuleComponent> CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    EEffectRemovalPolicy EffectRemovalPolicy { EEffectRemovalPolicy::RemoveOnEndOverlap };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    float EffectLevel { 1.f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    bool bApplyEffectsToEnemies { false };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    bool bDamageEffect { false };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypesMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> StackingTypesMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    bool bFloating { false };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    float FloatingSpeed { 2.5f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    float FloatingRange { 30.f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    float RotationSpeed { 50.f };

    FVector SourceLocation;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent *OverlappedComponent,
                   AActor *OtherActor, UPrimitiveComponent *OtherComp,
                   int32 OtherBodyIndex, bool bFromSweep,
                   const FHitResult &SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent *OverlappedComponent,
                      AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);

private:
    TMap<uint32, FActiveGameplayEffectHandle> ActorActiveEffectsMap;

    FActiveGameplayEffectHandle
    ApplyEffect(AActor *TargetActor, FGameplayEffectSpecHandle &EffectSpec,
                TSubclassOf<UGameplayEffect> GamePlayEffectClass);

    float RunningTime { 0.f };
};
