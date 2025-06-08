// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "Utils/GameplayAbilityUtils.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}


void ABaseCharacter::InitAbilityActorInfo() {
}

void ABaseCharacter::InitPrimaryAttributes() {
    check(IsValid(GetAbilitySystemComponent()) && DefaultPrimaryAttributesEffect);

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, DefaultPrimaryAttributesEffect, 1.f);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::InitSecondaryAttributes() {
    check(IsValid(GetAbilitySystemComponent()) && DefaultSecondaryAttributesEffect);

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, DefaultSecondaryAttributesEffect, 1.f);
    // GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::InitVitalAttributes() {
    check(IsValid(GetAbilitySystemComponent()) && DefaultVitalAttributesEffect);

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, DefaultVitalAttributesEffect, 1.f);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::AddCharactorAbilities() {
    if (!HasAuthority()) {
        return;
    }

    auto DefaultAbilitySystemComponent = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);
    DefaultAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

FVector ABaseCharacter::GetCombatSocketLocation() {
    return Weapon->GetSocketLocation(WeaponTipSocketName);
}
