// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "EffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
    DoNotRemove,
    RemoveOnEndOverlap
};

class UGameplayEffect;
struct FGameplayEffectSpecHandle;

UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

    UFUNCTION(BlueprintCallable)
    TOptional<FGameplayEffectSpecHandle> ConstructEffectSpec(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass);

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass);

    UFUNCTION(BlueprintCallable)
    void RemoveActiveEffect(AActor* TargetActor);

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    EEffectRemovalPolicy EffectRemovalPolicy { EEffectRemovalPolicy::RemoveOnEndOverlap };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
    float EffectLevel { 1.f };

private:
    TMap<uint32, FActiveGameplayEffectHandle> ActorActiveEffectsMap;

    FActiveGameplayEffectHandle ApplyEffect(AActor* TargetActor, FGameplayEffectSpecHandle EffectSpec, TSubclassOf<UGameplayEffect> GamePlayEffectClass);
};
