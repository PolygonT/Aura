// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

class UDefaultUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()
	
public:

    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Param);

    UAttributeMenuWidgetController *
    GetAttributeMenuWidgetController(const FWidgetControllerParams &Param);

    void InitOverlay(APlayerController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:

    UPROPERTY()
    TObjectPtr<UDefaultUserWidget> OverlayWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UDefaultUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
