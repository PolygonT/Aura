// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DefaultProjectileSpell.h"
#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/DefaultProjectile.h"
#include "DefaultGameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// void UDefaultProjectileSpell::ActivateAbility(
//     const FGameplayAbilitySpecHandle Handle,
//     const FGameplayAbilityActorInfo *ActorInfo,
//     const FGameplayAbilityActivationInfo ActivationInfo,
//     const FGameplayEventData *TriggerEventData) {
//
//     Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//
// }

void UDefaultProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const AActor* HomingTarget, const float PitchOverride) {
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

    if (!bIsServer) {
        return;
    }

    APawn* AbilityInstigator = Cast<APawn>(GetAvatarActorFromActorInfo());
    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

    if (!AbilityInstigator) {
        return;
    }

    const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
        GetAvatarActorFromActorInfo(), FDefaultGameplayTags::Get().Montage_Attack_Weapon);
    FVector ProjectileLaunchVector = ProjectileTargetLocation - SocketLocation;

    float HalfMaxRotationDegree {30.f};

    //飞行物数量
    int32 Nums = ProjectileNums.GetValueAtLevel(GetAbilityLevel());
    float MaxiumnAngle {60.f};
    float BestAngle {10.f};

    TArray<FVector> SpawnVectors = UDefaultAbilitySystemLibrary::GetSpawnVectorsByNums(ProjectileLaunchVector, MaxiumnAngle, BestAngle, Nums);

    for (const auto& SpawnVector : SpawnVectors) {
        FRotator Rotation = SpawnVector.Rotation();
        Rotation.Pitch = PitchOverride;
        SpawnAngledProjectile(Rotation, SocketLocation, AbilityInstigator, HomingTarget, ProjectileTargetLocation);
    }

    // if ((Nums - 1) * BestAngle <= MaxiumnAngle) {
    //     // Best Angle Works
    //     float LeftMostAngle = -((Nums - 1) * BestAngle) / 2;
    //     FRotator LeftRotator = FRotator {0.f, LeftMostAngle, 0.f};
    //     FVector LeftMostVector = LeftRotator.RotateVector(ProjectileLaunchVector);
    //
    //     for (int32 i = 0; i < Nums; i++) {
    //         FRotator NextRotator = FRotator {0.f, i * BestAngle, 0.f};
    //         FRotator Rotation = NextRotator.RotateVector(LeftMostVector).Rotation();
    //         Rotation.Pitch = PitchOverride;
    //
    //         // Spawn Projectile
    //         SpawnAngledProjectile(Rotation, SocketLocation, AbilityInstigator, HomingTarget, ProjectileTargetLocation);
    //     }
    // }
}



void UDefaultProjectileSpell::SpawnAngledProjectile(const FRotator Rotation, const FVector SocketLocation, 
                                                    APawn* AbilityInstigator, const AActor* HomingTarget, 
                                                    const FVector ProjectileTargetLocation) {
    FTransform SpawnTransform { SocketLocation, };
    SpawnTransform.SetRotation(Rotation.Quaternion());

    ADefaultProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADefaultProjectile>(
        ProjectileClass, SpawnTransform, 
        GetOwningActorFromActorInfo(), CastChecked<APawn>(GetAvatarActorFromActorInfo()), 
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    Projectile->SetInstigator(AbilityInstigator);
    Projectile->GameplayAbility = this;

    UAbilitySystemComponent* SourceASC = 
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();

    ContextHandle.AddSourceObject(Projectile);
    const FGameplayEffectSpecHandle SpecHandle = 
        SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);


    // Assign Set By Caller
    for (auto& [DamageTypeTag, DamageValueScalable] : DamageTypesMap) {
        const float ScaledDamage = DamageValueScalable.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, ScaledDamage);
    }

    for (auto& [StackingTypeTag, StackingTypeValueScalable] : StackingTypesMap) {
        const float ScaledStackingVal = StackingTypeValueScalable.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, StackingTypeTag, ScaledStackingVal);
    }

    Projectile->DamageEffectSpecHandle = SpecHandle;

    if (HomingTarget && HomingTarget->Implements<UCombatInterface>()) {
        Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
    } else {
        // 将HomingTarget绑定到Projectile对象上，被GC识别到。因为ProjectileMovement->HomingTargetComponent是一个WeakPtr
        Projectile->HomingTarget = NewObject<USceneComponent>(USceneComponent::StaticClass());
        Projectile->HomingTarget->SetWorldLocation(ProjectileTargetLocation);
        Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTarget;
    }

    Projectile->ProjectileMovement->bIsHomingProjectile = bIsHomingProjectile;
    Projectile->ProjectileMovement->HomingAccelerationMagnitude = HomingAcceleration;

    Projectile->FinishSpawning(SpawnTransform);
}
