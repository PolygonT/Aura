// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UObject> WidgetController;

    UFUNCTION(BlueprintCallable)
    void SetWidgetController(UObject* InWidgetController);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void WidgetControllerSet();
};
