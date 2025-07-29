// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
#include "Containers/Map.h"
#include "DefaultGameplayTags.h"
#include "GameplayCueManager.h"
#include "GameplayPrediction.h"
#include "Utils/GameplayAbilityUtils.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

    
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
    CollisionComponent->SetupAttachment(GetRootComponent());
    CollisionComponent->SetCapsuleHalfHeight(22.f);

    if (HasAuthority()) {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
        CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
    }

    for (auto& [DamageTypeTag, _] : FDefaultGameplayTags::Get().DamageTypeAndResistanceMap) {
        FGameplayTag& DamageType = DamageTypeTag;
        DamageTypesMap.Add(DamageType, {});
    }

    for (const auto& [StackingTypeTag, _] : FDefaultGameplayTags::Get().StackingTypeAndTriggeredMap) {
        StackingTypesMap.Add(StackingTypeTag, {});
    }
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

    SourceLocation = GetActorLocation();
}

void AEffectActor::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    if (bFloating) {
        RunningTime += DeltaSeconds;
        float alpha = 0.5f * FMath::Sin(RunningTime * FloatingSpeed) + 0.5f; // alpha 0 ~ 1
        const FVector TargetLocation = { SourceLocation.X, SourceLocation.Y, SourceLocation.Z + FloatingRange};
        const FVector DesitinationLocation = FMath::Lerp(SourceLocation, TargetLocation, alpha);

        const FRotator Rotation = { 0.f, DeltaSeconds * RotationSpeed, 0.f };
        AddActorWorldRotation(Rotation);
        SetActorLocation(DesitinationLocation);
    }
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass) {
    if (TargetActor->ActorHasTag("Enemy") && !bApplyEffectsToEnemies) {
        return;
    }
    // TODO 1: USE EffectSpecOpt.GetPtrOrNull() will cause a crash, don't known why
    auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    if (!TargetASC) {
        return;
    }

    auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
    SourceASC = SourceASC ? SourceASC : TargetASC;

    auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
        this, SourceASC, TargetASC, GamePlayEffectClass, EffectLevel);

    if (!EffectSpec) return;


    if (bDamageEffect) {
        for (auto& Pair : DamageTypesMap) {
            const float ScaledDamage = Pair.Value.GetValueAtLevel(EffectLevel);
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(*EffectSpec, Pair.Key, ScaledDamage);
        }

        for (auto& Pair : StackingTypesMap) {
            const float ScaledStackingVal = Pair.Value.GetValueAtLevel(EffectLevel);
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(*EffectSpec, Pair.Key, ScaledStackingVal);
        }
    }
   
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
    // FScopedPredictionWindow Window { TargetAbilitySystemComponent };
    // UAbilitySystemGlobals::Get().GetGameplayCueManager()->InvokeGameplayCueExecuted_WithParams(
    //     TargetAbilitySystemComponent, GEGameplayCueTag, FPredictionKey(), CueParam);

    // FGameplayCueParameters CueParam {};
    //
    // UAbilitySystemGlobals::Get().InitGameplayCueParameters(CueParam, EffectSpec.Data->GetContext());
    // TargetAbilitySystemComponent->ExecuteGameplayCue(GEGameplayCueTag, EffectSpec.Data->GetContext());

    FActiveGameplayEffectHandle ActiveEffectHandle =  TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
        *EffectSpec.Data.Get());

    return ActiveEffectHandle;
}

void AEffectActor::RemoveActiveEffect(AActor* TargetActor) {
    if (EffectRemovalPolicy != EEffectRemovalPolicy::RemoveOnEndOverlap) {
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
    if (!HasAuthority()) { return; }

    RemoveActiveEffect(OtherActor);
}

TMap<FGameplayTag, FScalableFloat> AEffectActor::GetDamageTypesMap() const {
    return DamageTypesMap;
}

TMap<FGameplayTag, FScalableFloat> AEffectActor::GetStackingTypesMap() const {
    return StackingTypesMap;
}

