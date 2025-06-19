// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
#include "DefaultAbilityTypes.h"
#include "Game/DefaultGameModeBase.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DefaultPlayerState.h"
#include "UI/HUD/DefaultHUD.h"
#include "UI/WidgetController/DefaultWidgetController.h"

UOverlayWidgetController *
UDefaultAbilitySystemLibrary::GetOverlayWidgetController(
    const UObject *WorldContextObject) {
    auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (!PC) {
        return nullptr;
    }

    auto DefaultHUD = Cast<ADefaultHUD>(PC->GetHUD());

    if (!DefaultHUD) {
        return nullptr;
    }

    auto DefaultPlayerState = PC->GetPlayerState<ADefaultPlayerState>();

    auto AbilitySystemComponent = DefaultPlayerState->GetAbilitySystemComponent();

    auto AttributeSet = DefaultPlayerState->GetAttributeSet();

    const auto Params = 
        FWidgetControllerParams(PC, DefaultPlayerState, AbilitySystemComponent, AttributeSet);
    return DefaultHUD->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController *
UDefaultAbilitySystemLibrary::GetAttributeMenuWidgetController(
    const UObject *WorldContextObject) {
    auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (!PC) {
        return nullptr;
    }

    auto DefaultHUD = Cast<ADefaultHUD>(PC->GetHUD());

    if (!DefaultHUD) {
        return nullptr;
    }

    auto DefaultPlayerState = PC->GetPlayerState<ADefaultPlayerState>();

    auto AbilitySystemComponent = DefaultPlayerState->GetAbilitySystemComponent();

    auto AttributeSet = DefaultPlayerState->GetAttributeSet();

    const auto Params = 
        FWidgetControllerParams(PC, DefaultPlayerState, AbilitySystemComponent, AttributeSet);
    return DefaultHUD->GetAttributeMenuWidgetController(Params);

}

// void UDefaultAbilitySystemLibrary::InitializeDefaultAttributes(
//     const UObject *WorldContextObject, ECharacterClass CharacterClass,
//     float Level) {
//     
//     auto GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
//
//     if (!GameMode) { return; }
//
//     auto ClassInfo = GameMode->CharacterClassInfo;
//     auto ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
// }

UCharacterClassInfo *UDefaultAbilitySystemLibrary::GetCharacterClassInfo(
    const UObject *WorldContextObject) {
    auto GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

    if (!GameMode) {
        return nullptr;
    }

    return GameMode->CharacterClassInfo;
}

bool UDefaultAbilitySystemLibrary::IsBlockedHit(
    const FGameplayEffectContextHandle &ContextHandle) {

    if (const FDefaultGameplayEffectContext* DefaultContext = GetDefaultContext(ContextHandle)) {
        return DefaultContext->IsBlockedHit();
    }

    return false;

}

bool UDefaultAbilitySystemLibrary::IsCriticalHit(
    const FGameplayEffectContextHandle &ContextHandle) {

    if (const FDefaultGameplayEffectContext* DefaultContext = GetDefaultContext(ContextHandle)) {
        return DefaultContext->IsCriticalHit();
    }

    return false;
}

void UDefaultAbilitySystemLibrary::SetIsBlockedHit(
    FGameplayEffectContextHandle &ContextHandle, bool bInIsBlockedHit) {

    if (FDefaultGameplayEffectContext* DefaultContext = GetDefaultContextNonConst(ContextHandle)) {
        DefaultContext->SetIsBlockedHit(bInIsBlockedHit);
    }
}

void UDefaultAbilitySystemLibrary::SetIsCriticalHit(
    FGameplayEffectContextHandle &ContextHandle, bool bInIsCriticalHit) {

    if (FDefaultGameplayEffectContext* DefaultContext = GetDefaultContextNonConst(ContextHandle)) {
        DefaultContext->SetIsCriticalHit(bInIsCriticalHit);
    }
}

const FDefaultGameplayEffectContext*
UDefaultAbilitySystemLibrary::GetDefaultContext(
    const FGameplayEffectContextHandle &ContextHandle) {

    return static_cast<const FDefaultGameplayEffectContext*>(ContextHandle.Get());
}

FDefaultGameplayEffectContext *
UDefaultAbilitySystemLibrary::GetDefaultContextNonConst(
    FGameplayEffectContextHandle &ContextHandle) {

    return static_cast<FDefaultGameplayEffectContext*>(ContextHandle.Get());
}
