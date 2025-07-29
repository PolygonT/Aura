// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AbilityWidgetController.h"

UOverlayWidgetController* ADefaultHUD::GetOverlayWidgetController(const FWidgetControllerParams& Param) {

    if (!OverlayWidgetController) {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerMemberVariables(Param);
    }

    return OverlayWidgetController;
}

UAttributeMenuWidgetController *ADefaultHUD::GetAttributeMenuWidgetController(
    const FWidgetControllerParams &Param) {
    if (!AttributeMenuWidgetController) {
        AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
        AttributeMenuWidgetController->SetWidgetControllerMemberVariables(Param);
    }

    return AttributeMenuWidgetController;
}

UAbilityWidgetController *
ADefaultHUD::GetAbilityWidgetController(const FWidgetControllerParams &Param) {

    if (!AbilityWidgetController) {
        AbilityWidgetController = NewObject<UAbilityWidgetController>(this, AbilityWidgetControllerClass);
        AbilityWidgetController->SetWidgetControllerMemberVariables(Param);
    }

    return AbilityWidgetController;
}

void ADefaultHUD::InitOverlay(
        APlayerController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) {
    checkf(OverlayWidgetClass, TEXT("OverlayWidget not set, please fill BP_DefaultHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass not set, please fill BP_DefaultHUD"));

    auto Widget = CreateWidget<UDefaultUserWidget>(AC, OverlayWidgetClass);

    auto Param = FWidgetControllerParams(AC, PS, ASC, AS);

    auto LocalOverlayWidgetController = GetOverlayWidgetController(Param);
    auto LocalAttributeMenuWidgetController = GetAttributeMenuWidgetController(Param);
    auto LocalAbilityWidgetController = GetAbilityWidgetController(Param);
    Widget->SetWidgetController(LocalOverlayWidgetController);
    LocalOverlayWidgetController->BroadcastInitialValues();
    LocalOverlayWidgetController->BindCallbacksToDependencies();
    LocalAttributeMenuWidgetController->BindCallbacksToDependencies();
    LocalAbilityWidgetController->BindCallbacksToDependencies();

    Widget->AddToViewport();
}
