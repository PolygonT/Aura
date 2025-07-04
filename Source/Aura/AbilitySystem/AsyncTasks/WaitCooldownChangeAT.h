// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChangeAT.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS()
class AURA_API UWaitCooldownChangeAT : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FCooldownChangeSignature CooldownStart;

    UPROPERTY(BlueprintAssignable)
    FCooldownChangeSignature CooldownEnd;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UWaitCooldownChangeAT *
    WaitForCooldownChange(UAbilitySystemComponent *AbilitySystemComponent,
                          const FGameplayTag &InCooldownTag);

    UFUNCTION(BlueprintCallable)
    void EndTask();

protected:
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayTag CooldownTag;

    void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

    void OnActiveEffectAdded(UAbilitySystemComponent *TargetASC,
                             const FGameplayEffectSpec &SpecApplied,
                             FActiveGameplayEffectHandle ActiveEffectHandle);
};
