// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "Aura.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "DefaultGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "Utils/GameplayAbilityUtils.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetCapsuleComponent()->SetGenerateOverlapEvents(false);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ProjectileChannel, ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);

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
    check(CharacterClassInfo);
    check(IsValid(GetAbilitySystemComponent()));
    auto Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, Info.PrimaryAttributesEffect, GetPlayerLevel());
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::InitSecondaryAttributes() {
    check(CharacterClassInfo);
    check(IsValid(GetAbilitySystemComponent()));

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, CharacterClassInfo->SecondaryAttributesEffect, 1.f);
    // GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::InitVitalAttributes() {
    check(CharacterClassInfo);
    check(IsValid(GetAbilitySystemComponent()));

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, CharacterClassInfo->VitalAttributesEffect, 1.f);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void ABaseCharacter::AddCharactorGameplayCues() {
    GetAbilitySystemComponent()->AddGameplayCue_MinimalReplication(FDefaultGameplayTags::Get().GameplayCue_Test);
}

void ABaseCharacter::AddCharactorAbilities() {
    if (!HasAuthority()) {
        return;
    }

    auto DefaultAbilitySystemComponent = CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent);
    DefaultAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
    DefaultAbilitySystemComponent->AddCharacterAbilitiesNormal(NormalAbilities);
    if (SpecializedAbilities.Contains(CharacterClass)) {
        FSpecializedAbilityInfo SpecializedAbilitiesInfo = SpecializedAbilities[CharacterClass];
        DefaultAbilitySystemComponent->AddCharacterAbilitiesNormal(SpecializedAbilitiesInfo.Abilities);
    }
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) {

    if (MontageTag.MatchesTagExact(FDefaultGameplayTags::Get().Montage_Attack_Weapon)) {

        check(Weapon);
        return Weapon->GetSocketLocation(WeaponTipSocketName);
    } else if (MontageTag.MatchesTagExact(FDefaultGameplayTags::Get().Montage_Attack_LeftHand)) {

        return GetMesh()->GetSocketLocation(LeftHandSocketName);
    } else if (MontageTag.MatchesTagExact(FDefaultGameplayTags::Get().Montage_Attack_RightHand)) {

        return GetMesh()->GetSocketLocation(RightHandSocketName);
    } else {
        checkf(false, TEXT("invalid MontageTag as Input Param"));
        // !not reachable!
        return {};
    }
}

UAnimMontage *ABaseCharacter::GetHitReactMontage_Implementation() {
    return HitReactMontage;
}

void ABaseCharacter::Die() { 
    MulticastHandleDealth();
}

void ABaseCharacter::MulticastHandleDealth_Implementation() {
    Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    Weapon->SetEnableGravity(true);
    Weapon->SetSimulatePhysics(true);
    Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    auto _Mesh = GetMesh();
    _Mesh->SetEnableGravity(true);
    _Mesh->SetSimulatePhysics(true);
    _Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    _Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Dissolve();

    bDead = true;
}

void ABaseCharacter::Dissolve() {
    TArray<UMaterialInstanceDynamic*> arr {}; 
    if (IsValid(DissolveMaterialInstance)) {

        UMaterialInstanceDynamic* DynamicMatInst = 
            UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
        GetMesh()->SetMaterial(0, DynamicMatInst);

        arr.Add(DynamicMatInst);
    }
    if (IsValid(WeaponDissolveMaterialInstance)) {
        UMaterialInstanceDynamic* WeaponDynamicMatInst = 
            UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);

        Weapon->SetMaterial(0, WeaponDynamicMatInst);
        arr.Add(WeaponDynamicMatInst);

    }

    if (arr.Num() > 0) {
        StartDissolveTimeline(arr);
    }
}

bool ABaseCharacter::IsDead_Implementation() const {
    return bDead;
}

AActor *ABaseCharacter::GetAvtar_Implementation() {
    return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontages_Implementation() {
    return AttackMontages;
}

UNiagaraSystem *ABaseCharacter::GetBloodEffect_Implementation() {
    return BloodEffect;
}


