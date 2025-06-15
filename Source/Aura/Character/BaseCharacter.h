// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemInterface.h"
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

UCLASS(Abstract)
class AURA_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

    // =============== Combat Interface ===================
    virtual UAnimMontage *GetHitReactMontage_Implementation() override;

    virtual void Die() override;
    
    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastHandleDealth();

    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& MaterialInstance);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combat")
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

    UPROPERTY(EditAnywhere)
    FName WeaponTipSocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    ECharacterClass CharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "Init Defaults")
    TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

    virtual FVector GetCombatSocketLocation() override;

    virtual void InitAbilityActorInfo();

    void InitPrimaryAttributes();

    void InitSecondaryAttributes();

    void InitVitalAttributes();

    void AddCharactorAbilities();

private:
    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> NormalAbilities;
    //
    // UPROPERTY(EditAnywhere, Category = "Abilities")
    // TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
};
