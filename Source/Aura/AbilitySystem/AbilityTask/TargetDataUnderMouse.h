// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Ability|Task", 
              meta = (DisPlayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = true))
    static UTargetDataUnderMouse *
    CreateTargetDataUnderMouse(UGameplayAbility *OwningAbility);

    UPROPERTY(BlueprintAssignable)
    FMouseTargetDataSignature ValidData;

private:
    virtual void Activate() override;

    void SendMouseCursorData();

    void OnTargetDataReplicatedCallback(
        const FGameplayAbilityTargetDataHandle &DataHandle,
        FGameplayTag ActivationTag);
};
