// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChangeAT.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"

UWaitCooldownChangeAT *UWaitCooldownChangeAT::WaitForCooldownChange(
    UAbilitySystemComponent *AbilitySystemComponent,
    const FGameplayTag &InCooldownTag) {
    UWaitCooldownChangeAT* WaitCooldownChange = NewObject<UWaitCooldownChangeAT>();

    if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid()) {
        WaitCooldownChange->EndTask();
        return nullptr;
    }

    WaitCooldownChange->ASC = AbilitySystemComponent;
    WaitCooldownChange->CooldownTag = InCooldownTag;

    AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(
            WaitCooldownChange, &ThisClass::CooldownTagChanged);
    AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChangeAT::OnActiveEffectAdded);

    return WaitCooldownChange;
}

void UWaitCooldownChangeAT::EndTask() {
    if (!IsValid(ASC)) {
        return;
    }

    ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
        .RemoveAll(this);

    SetReadyToDestroy();
    MarkAsGarbage();
}

void UWaitCooldownChangeAT::CooldownTagChanged(
    const FGameplayTag InCooldownTag, int32 NewCount) {
    if (NewCount == 0) {
        CooldownEnd.Broadcast(0.f);
    }
}

void UWaitCooldownChangeAT::OnActiveEffectAdded(
    UAbilitySystemComponent *TargetASC, const FGameplayEffectSpec &SpecApplied,
    FActiveGameplayEffectHandle ActiveEffectHandle) {
    FGameplayTagContainer AssetTags;
    SpecApplied.GetAllAssetTags(AssetTags);

    FGameplayTagContainer GrantedTags;
    SpecApplied.GetAllGrantedTags(GrantedTags);

    if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag)) {
        FGameplayEffectQuery GameplayEffectQuery 
            = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());

        TArray<float> TimesRemainingArr = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

        float TimeRemaining {};
        if (TimesRemainingArr.Num() == 0) {
            TimeRemaining = TimesRemainingArr[0];
        } else if (TimesRemainingArr.Num() > 1) {
            TimesRemainingArr.Sort();
            TimeRemaining = TimesRemainingArr[TimesRemainingArr.Num() - 1];
        }

        CooldownStart.Broadcast(TimeRemaining);
    }

}

