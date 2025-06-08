// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
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
