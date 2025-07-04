// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "Containers/Map.h"
#include "GameplayCueManager.h"
#include "GameplayPrediction.h"
#include "Utils/GameplayAbilityUtils.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetupAttachment(GetRootComponent());

    if (HasAuthority()) {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
        CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
    }
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
    auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
        this, TargetASC, TargetASC, GamePlayEffectClass, EffectLevel);

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

FActiveGameplayEffectHandle
AEffectActor::ApplyEffect(AActor *TargetActor,
                          FGameplayEffectSpecHandle& EffectSpec,
                          TSubclassOf<UGameplayEffect> GamePlayEffectClass) {

    auto TargetAbilitySystemComponent =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    FScopedPredictionWindow Window { TargetAbilitySystemComponent };

    FGameplayCueParameters CueParam {};

    UAbilitySystemGlobals::Get().GetGameplayCueManager()->InvokeGameplayCueExecuted_WithParams(
        TargetAbilitySystemComponent, GEGameplayCueTag, TargetAbilitySystemComponent->ScopedPredictionKey, CueParam);

    FActiveGameplayEffectHandle ActiveEffectHandle =  TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
        *EffectSpec.Data.Get());

    return ActiveEffectHandle;
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
void AEffectActor::OnOverlap(UPrimitiveComponent *OverlappedComponent,
                             AActor *OtherActor, UPrimitiveComponent *OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult &SweepResult) {
    ApplyEffectToTarget(OtherActor, GameplayEffectClass);
}

void AEffectActor::OnEndOverlap(UPrimitiveComponent *OverlappedComponent,
                                AActor *OtherActor,
                                UPrimitiveComponent *OtherComp,
                                int32 OtherBodyIndex) {

    if (EffectRemovalPolicy != EEffectRemovalPolicy::RemoveOnEndOverlap) {
        return;
    }

    RemoveActiveEffect(OtherActor);
}

