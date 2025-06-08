// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DefaultProjectileSpell.h"
#include "Actor/DefaultProjectile.h"
#include "Engine/World.h"
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

void UDefaultProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation) {
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

    if (!bIsServer) {
        return;
    }

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

    if (!CombatInterface) {
        return;
    }

    const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
    FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
    Rotation.Pitch = 0.f;

    FTransform SpawnTransform { SocketLocation, };
    SpawnTransform.SetRotation(Rotation.Quaternion());

    auto Projectile = GetWorld()->SpawnActorDeferred<ADefaultProjectile>(
        ProjectileClass, SpawnTransform, 
        GetOwningActorFromActorInfo(), CastChecked<APawn>(GetAvatarActorFromActorInfo()), 
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


    Projectile->FinishSpawning(SpawnTransform);
}

