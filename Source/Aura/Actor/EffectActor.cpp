// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Containers/Map.h"
#include "Utils/GameplayAbilityUtils.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass) {
    if (TargetActor->ActorHasTag("Enemy") && !bApplyEffectsToEnemies) {
        return;
    }
    // TODO 1: USE EffectSpecOpt.GetPtrOrNull() will cause a crash, don't known why
    auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(this, TargetActor, GamePlayEffectClass, EffectLevel);
    if (!EffectSpec) return;
   
    // auto EffectSpec = EffectSpecOpt.GetValue();
    auto DurationPolicy = EffectSpec->Data->Def->DurationPolicy;

    switch (DurationPolicy) {
        case EGameplayEffectDurationType::Instant :
        case EGameplayEffectDurationType::HasDuration :
            {
                ApplyEffect(TargetActor, *EffectSpec, GamePlayEffectClass);
                break;
            }
        case EGameplayEffectDurationType::Infinite :
            {
                auto ActiveEffect = ApplyEffect(TargetActor, *EffectSpec, GamePlayEffectClass);

                if (EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
                    ActorActiveEffectsMap.Add(TargetActor->GetUniqueID(), ActiveEffect);
                }

                break;
            }
        default:
            break;
    }

    if (DurationPolicy != EGameplayEffectDurationType::Infinite) {
        Destroy();
    }
}

FActiveGameplayEffectHandle AEffectActor::ApplyEffect(AActor* TargetActor, FGameplayEffectSpecHandle EffectSpec, TSubclassOf<UGameplayEffect> GamePlayEffectClass) {
    auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    return TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void AEffectActor::RemoveActiveEffect(AActor* TargetActor) {
    if (EffectRemovalPolicy == EEffectRemovalPolicy::DoNotRemove) {
        return;
    }

    auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    auto TargetActorUID = TargetActor->GetUniqueID();

    if (ActorActiveEffectsMap.Contains(TargetActorUID)) {
        auto ActiveEffect = ActorActiveEffectsMap[TargetActorUID];
        TargetAbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffect, 1);
        // TargetAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, )

        ActorActiveEffectsMap.Remove(TargetActorUID);
    }
}
