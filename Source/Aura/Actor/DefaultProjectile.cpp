// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DefaultProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura.h"
#include "Components/SphereComponent.h"
#include "DefaultGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Utils/GameplayAbilityUtils.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"

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
    auto LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
    LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void ADefaultProjectile::Destroyed() {
    if (!bHit && !HasAuthority()) {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
    }

    Super::Destroyed();
}

void ADefaultProjectile::OnSphereOverlap(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {

    if (!bHit) {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
    }

    if (HasAuthority()) {
        if (auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)) {
            // Cause Damge
            // TUniquePtr<FGameplayEffectSpecHandle> EffectSpec = 
            //     GameplayAbilityUtils::ConstructEffectSpec(this, OtherActor, DamageEffectClass, 1.f);

            TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
        }

        Destroy();
    } else {
        bHit = true;
    }
}

