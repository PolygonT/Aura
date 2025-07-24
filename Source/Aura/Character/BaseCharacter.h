// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemInterface.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class UGameplayAbility;
struct FGameplayAbilitySpec;

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
    MainState,
    CombatState
};

USTRUCT(BlueprintType)
struct FSpecializedAbilityInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> Abilities;
};

UCLASS(Abstract)
class AURA_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    
    // =============== Combat Interface ===================
    virtual UAttributeSet *GetAttributeSet() const override;
    virtual UAnimMontage *GetHitReactMontage_Implementation() override;
    virtual void Die() override;
    virtual FVector GetCombatSocketLocation_Implementation(
        const FGameplayTag &MontageTag) override;
    virtual bool IsDead_Implementation() const override;
    virtual AActor *GetAvtar_Implementation() override;
    virtual TArray<FTaggedMontage>
    GetAttackMontages_Implementation() override;
    virtual UNiagaraSystem *GetBloodEffect_Implementation() override;
    virtual TSubclassOf<UGameplayEffect> GetOnFireEffect() const override;
    virtual TSubclassOf<UGameplayEffect> GetOnLightningEffect() const override;
    virtual TSubclassOf<UGameplayEffect> GetLevelUpEffect() const override;
    virtual float GetXpDrop() const override;
    virtual int32 GetCurrentMaxXp() const override;
    virtual int32 GetPlayerLevel() const override;
    virtual bool IsPlayer() const override;

    virtual void StartCombatState_Implementation(ECombatState InCombatState) override;
    virtual void EndCombatState_Implementation() override;

    // =============== Combat Interface ===================

    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastHandleDealth();

    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void StartDissolveTimeline(
        const TArray<UMaterialInstanceDynamic *> &MaterialInstance);

    UFUNCTION(BlueprintCallable)
    const UGameplayAbility* GetActivatableAbilityByTag(UPARAM(ref) FGameplayTag& Tag, FGameplayAbilitySpec& OutAbilitySpec);

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FTaggedMontage> AttackMontages;

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    // UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    // TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffect;
    //
    // UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    // TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffect;
    //
    // UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    // TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffect;

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName WeaponTipSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName LeftHandSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName RightHandSocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    ECharacterClass CharacterClass;

    UPROPERTY(BlueprintReadOnly)
    ECharacterState CharacterState { ECharacterState::MainState };

    UPROPERTY(BlueprintReadOnly)
    ECombatState CombatState { ECombatState::Default };

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    TSubclassOf<UGameplayEffect> OnFireEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    TSubclassOf<UGameplayEffect> OnLightningEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    TSubclassOf<UGameplayEffect> LevelUpEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    FScalableFloat XpDropScalable;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    FScalableFloat MaxXpScalable;

    bool bDead{false};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpped { 250.f };

    virtual void InitAbilityActorInfo();

    void InitPrimaryAttributes();

    void InitSecondaryAttributes();

    void InitVitalAttributes();

    void InitReducingStackingEffect();

    void AddCharactorAbilities();

    void AddCharactorGameplayCues();

    void Stun(const FGameplayTag GameplayTag, int32 NewCount);

  private:
    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> NormalAbilities;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    TMap<ECharacterClass, FSpecializedAbilityInfo> SpecializedAbilities;
    //
    // UPROPERTY(EditAnywhere, Category = "Abilities")
    // TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UNiagaraSystem> BloodEffect;
};
