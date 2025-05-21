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

/**
 * 
 */
UCLASS()
class AURA_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()
	
public:

    UPROPERTY()
    TObjectPtr<UDefaultUserWidget> OverlayWidget;

    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Param);

    void InitOverlay(APlayerController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UDefaultUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
