// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DefaultProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/DefaultProjectile.h"
#include "DefaultGameplayTags.h"
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

    APawn* AbilityInstigator = Cast<APawn>(GetAvatarActorFromActorInfo());
    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

    if (!AbilityInstigator) {
        return;
    }

    const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
        GetAvatarActorFromActorInfo(), FDefaultGameplayTags::Get().Montage_Attack_Weapon);
    FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
    Rotation.Pitch = 0.f;

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

   const FGameplayEffectSpecHandle SpecHandle = 
        SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());


    // Assign Set By Caller
    for (auto& Pair : DamageTypesMap) {
        const float ScaledDamage = Pair.Value.GetValueAtLevel(CombatInterface->GetPlayerLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    }

    Projectile->DamageEffectSpecHandle = SpecHandle;

    Projectile->FinishSpawning(SpawnTransform);
}

