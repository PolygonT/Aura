// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DefaultProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Aura.h"
#include "Components/SphereComponent.h"
#include "DefaultGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayPrediction.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Utils/GameplayAbilityUtils.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "GameplayCueFunctionLibrary.h"
#include "GameplayCueManager.h"

ADefaultProjectile::ADefaultProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    SetRootComponent(Sphere);

    Sphere->SetCollisionObjectType(ProjectileChannel);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    ProjectileMovement->InitialSpeed = 550.f;
    ProjectileMovement->MaxSpeed = 550.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ADefaultProjectile::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(LifeSpan);

    // LOOPING SOUND
    if (LoopingSound) {
        auto LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
        LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
    }
	
    if (HasAuthority()) {
        Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
    }
}

void ADefaultProjectile::Destroyed() {
    // UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    // UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

    Super::Destroyed();
}

void ADefaultProjectile::OnSphereOverlap(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {

    // 避免伤害自己
    if (!DamageEffectSpecHandle.Data.IsValid() || 
        DamageEffectSpecHandle.Data->GetContext().GetEffectCauser() == OtherActor) {
        return;
    }

    if (auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)) {

        TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
    }

    if (auto SourceASC = GameplayAbility->GetAbilitySystemComponentFromActorInfo()) {
        FScopedPredictionWindow PredictionWindow { SourceASC };
        FGameplayCueParameters CueParam {};
        CueParam.Location = GetActorLocation();
        UAbilitySystemGlobals::Get().GetGameplayCueManager()->InvokeGameplayCueExecuted_WithParams(
            SourceASC, FDefaultGameplayTags::Get().GameplayCue_RangeImpact, SourceASC->ScopedPredictionKey, CueParam);
    }

    Destroy();
}

