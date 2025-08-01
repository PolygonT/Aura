// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystem/Ability/DefaultGameplayAbility.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Aura.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "DefaultGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
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

    // Register Gameplay Event
    if (HasAuthority()) {
        AbilitySystemComponent->RegisterGameplayTagEvent(
            FDefaultGameplayTags::Get().Effect_Stun, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &ThisClass::Stun);
    }
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

void ABaseCharacter::InitReducingStackingEffect() {
    check(CharacterClassInfo);
    check(IsValid(GetAbilitySystemComponent()));

    auto EffectSpec = *GameplayAbilityUtils::ConstructEffectSpec(this, this, CharacterClassInfo->ReducingStackingEffect, 1.f);
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
        // checkf(false, TEXT("invalid MontageTag as Input Param"));
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

TSubclassOf<UGameplayEffect> ABaseCharacter::GetOnFireEffect() const {
    return OnFireEffect;
}

TSubclassOf<UGameplayEffect> ABaseCharacter::GetOnLightningEffect() const {
    return OnLightningEffect;
}

TSubclassOf<UGameplayEffect> ABaseCharacter::GetLevelUpEffect() const { 
    return LevelUpEffect; 
}

UAttributeSet *ABaseCharacter::GetAttributeSet() const { return AttributeSet; }

const UGameplayAbility* ABaseCharacter::GetActivatableAbilityByTag(FGameplayTag& Tag, FGameplayAbilitySpec& OutAbilitySpec) {

    // ABILITYLIST_SCOPE_LOCK();
    for (const auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities()) {
        UDefaultGameplayAbility *DefaultGameplayAbility = Cast<UDefaultGameplayAbility>(AbilitySpec.Ability);

        if (!DefaultGameplayAbility) { continue; }

        for (const auto& AssetTag : DefaultGameplayAbility->GetAssetTags()) {
            if (AssetTag.MatchesTagExact(Tag)) {
                OutAbilitySpec = AbilitySpec;
                return AbilitySpec.Ability;
            }
        }
    }

    return nullptr;
}

void ABaseCharacter::StartCombatState_Implementation(ECombatState InCombatState) {
    CharacterState = ECharacterState::CombatState;
    CombatState = InCombatState;
    switch (InCombatState) {
        case ECombatState::CastShockLoop :
        case ECombatState::Stun :
            GetCharacterMovement()->DisableMovement();
        break;
        default:
        break;
    }
}

void ABaseCharacter::EndCombatState_Implementation() {
    switch (CombatState) {
        case ECombatState::CastShockLoop :
        case ECombatState::Stun :
            GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        break;
        default:
        break;
    }

    CharacterState = ECharacterState::MainState;
    CombatState = ECombatState::Default;
}

void ABaseCharacter::Stun(const FGameplayTag GameplayTag, int32 NewCount) {
    bool bStun = NewCount > 0;

    if (bStun) {
        ICombatInterface::Execute_StartCombatState(this, ECombatState::Stun);
    } else {
        ICombatInterface::Execute_EndCombatState(this);
    }
}

int32 ABaseCharacter::GetCurrentMaxXp() const {
    return MaxXpScalable.GetValueAtLevel(GetPlayerLevel());
}

int32 ABaseCharacter::GetPlayerLevel() const {
    if (auto DefaultAS = Cast<UDefaultAttributeSet>(AttributeSet)) {
        return DefaultAS->GetLevel();
    }

    return 0;
}

bool ABaseCharacter::IsPlayer() const
{
    return false;
}

float ABaseCharacter::GetXpDrop() const {
    return XpDropScalable.GetValueAtLevel(GetPlayerLevel());
}

void ABaseCharacter::SetPlayerLevel(int32 InLevel) {
    _TempPlayerLevel = InLevel;
}

