// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DefaultHUD.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ADefaultHUD::GetOverlayWidgetController(const FWidgetControllerParams& Param) {

    if (!OverlayWidgetController) {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerMemberVariables(Param);
    }

    return OverlayWidgetController;
}

void ADefaultHUD::InitOverlay(
        APlayerController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) {
    checkf(OverlayWidgetClass, TEXT("OverlayWidget not set, please fill BP_DefaultHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass not set, please fill BP_DefaultHUD"));

    auto Widget = CreateWidget<UDefaultUserWidget>(GetWorld(), OverlayWidgetClass);

    auto Param = FWidgetControllerParams(AC, PS, ASC, AS);

    auto LocalOverlayWidgetController = GetOverlayWidgetController(Param);
    Widget->SetWidgetController(LocalOverlayWidgetController);
    LocalOverlayWidgetController->BroadcastInitialValues();
    LocalOverlayWidgetController->BindCallbacksToDependencies();
    Widget->AddToViewport();
}

