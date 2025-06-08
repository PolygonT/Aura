// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UDefaultInputConfig;
struct FGameplayTag;
class UDefaultAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ADefaultPlayerController();

protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    virtual void PlayerTick(float DeltaTime) override;

  private:
    // ==== MEMBER VIRIABLES ====
    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> ShiftAction;

    TScriptInterface<IEnemyInterface> LastFrameActor;

    TScriptInterface<IEnemyInterface> ThisFrameActor;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UDefaultInputConfig> InputConfig;

    TObjectPtr<UDefaultAbilitySystemComponent> DefaultAbilitySystemComponent;

    FVector CachedDestination { FVector::ZeroVector };

    float FollowTime { 0.f };

    float ShortPressThreshold { 0.5f };

    bool bAutoRunning { false };

    bool bTargeting { false };

    float AutoRunAcceptanceRadius { 50.f };

    TObjectPtr<USplineComponent> Spline;

    bool bShiftKeyDown = false;


    

    // ==== FUNCTIONS ====
    void Move(const FInputActionValue& InputActionValue);

    void CursorTrace();

    void AbilityInputTagPressed(const FInputActionValue &value, const FGameplayTag Tag);
    void AbilityInputTagReleased(const FInputActionValue &value, const FGameplayTag Tag);
    void AbilityInputTagHeld(const FInputActionValue &value, const FGameplayTag Tag);

    UDefaultAbilitySystemComponent *GetASC();

    void AutoRun();

    void ShiftPressed();
    void ShiftReleased();
};
