// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "GameplayPrediction.h"

UTargetDataUnderMouse *UTargetDataUnderMouse::CreateTargetDataUnderMouse(
    UGameplayAbility *OwningAbility) {
    UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

    return MyObj;
}

void UTargetDataUnderMouse::Activate() {
    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

    if (bIsLocallyControlled) {
        SendMouseCursorData();
    } else {
        AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(
            this, &ThisClass::OnTargetDataReplicatedCallback
        );
        bool b = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

        if (!b) {
            SetWaitingOnRemotePlayerData();
        }
    }

    // APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
    // FHitResult HitResult {};
    // Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
    // 
    // if (!HitResult.bBlockingHit) {
    //     return;
    // }

    // ValidData.Broadcast(HitResult.GetActor()->GetActorLocation());
}

void UTargetDataUnderMouse::SendMouseCursorData() {
    // MULTI PLAYER CODE
    FScopedPredictionWindow ScopedPrediction { AbilitySystemComponent.Get() };
    APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
    FHitResult HitResult {};
    Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

    // if (!HitResult.bBlockingHit) {
    //     return;
    // }

    FGameplayAbilityTargetDataHandle DataHandle;
    FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
    Data->HitResult = HitResult;
    DataHandle.Add(Data);

    AbilitySystemComponent->ServerSetReplicatedTargetData(
        GetAbilitySpecHandle(),
        GetActivationPredictionKey(),
        DataHandle,
        {},
        AbilitySystemComponent->ScopedPredictionKey
    );

    if (ShouldBroadcastAbilityTaskDelegates()) {
        ValidData.Broadcast(DataHandle);
    }
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(
    const FGameplayAbilityTargetDataHandle &DataHandle,
    FGameplayTag ActivationTag) {
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

    if (ShouldBroadcastAbilityTaskDelegates()) {
        ValidData.Broadcast(DataHandle);
    }
}

