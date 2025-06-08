// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultWidgetController.generated.h"

class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;


USTRUCT(BlueprintType)
struct FWidgetControllerParams {
    GENERATED_BODY()

    FWidgetControllerParams() {}

    FWidgetControllerParams(APlayerController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) 
        : PlayerController (AC), PlayerState (PS), AbilitySystemComponent (ASC), AttributeSet (AS) {}
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet;

    
};

/**
 * 
 */
UCLASS()
class AURA_API UDefaultWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetControllerMemberVariables(const FWidgetControllerParams& Param);

    UFUNCTION(BlueprintCallable)
    virtual void BroadcastInitialValues();

    UFUNCTION(BlueprintCallable)
    virtual void BindCallbacksToDependencies();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet;
};
