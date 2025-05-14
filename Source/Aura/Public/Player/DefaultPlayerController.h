// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

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

    TScriptInterface<IEnemyInterface> LastFrameActor;

    TScriptInterface<IEnemyInterface> ThisFrameActor;

    // ==== FUNCTIONS ====
    void Move(const FInputActionValue& InputActionValue);

    void CursorTrace();

};
