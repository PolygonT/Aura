// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "EffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
    DoNotRemove,
    RemoveOnEndOverlap
};

class UGameplayEffect;
struct FGameplayEffectSpecHandle;
class UShapeComponent;

UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass);

    UFUNCTION(BlueprintCallable)
    void RemoveActiveEffect(AActor* TargetActor);

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UShapeComponent> CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    EEffectRemovalPolicy EffectRemovalPolicy { EEffectRemovalPolicy::RemoveOnEndOverlap };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    float EffectLevel { 1.f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    bool bApplyEffectsToEnemies { false };

    UPROPERTY(EditAnywhere, Category = "Applied Effect")
    FGameplayTag GEGameplayCueTag;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                   UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                   bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent *OverlappedComponent,
                      AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);

private:
    TMap<uint32, FActiveGameplayEffectHandle> ActorActiveEffectsMap;

    FActiveGameplayEffectHandle
    ApplyEffect(AActor *TargetActor, FGameplayEffectSpecHandle &EffectSpec,
                TSubclassOf<UGameplayEffect> GamePlayEffectClass);
};
