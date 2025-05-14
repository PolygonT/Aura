// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DefaultPlayerController.h"
#include "Engine/HitResult.h"
#include "GameFramework/Pawn.h"
#include "Interaction/EnemyInterface.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ADefaultPlayerController::ADefaultPlayerController() {
    bReplicates = true;
}

void ADefaultPlayerController::BeginPlay() {
    Super::BeginPlay();

    check(InputMappingContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    Subsystem->AddMappingContext(InputMappingContext, 0);
    
    check(Subsystem);

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData {};
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void ADefaultPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Move);
}

void ADefaultPlayerController::Move(const FInputActionValue& InputActionValue) {
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation {0.f, Rotation.Yaw, 0.f};

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>()) {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}


void ADefaultPlayerController::PlayerTick(float DeltaTime) {
    Super::PlayerTick(DeltaTime);

    CursorTrace();
}

void ADefaultPlayerController::CursorTrace() {
    FHitResult CursorHit {};
    GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

    if (!CursorHit.bBlockingHit) return;


    LastFrameActor = ThisFrameActor;
    ThisFrameActor = CursorHit.GetActor();

    /**
     * A. if LastFrameActor & ThisFrameActor both null:
     *  do nothing
     * B. if LastFrameActor null & ThisFrameActor not null
     *  highlight ThisFrameActor
     * C. if LastFrameActor & ThisFrameActor bot not null
     *  C1. if LastFrameActor == ThisFrameActor
     *      do nothing  
     *  C2. if LastFrameActor != ThisFrameActor
     *      unhighlight LastFrameActor and highlight ThisFrameActor
     * D. if LastFrameActor not null & ThisFrameActor null
     *  unhighlight LastFrameActor
     **/

    if (!LastFrameActor && ThisFrameActor) {
        ThisFrameActor->HighlightActor();
    } else if (LastFrameActor && ThisFrameActor && LastFrameActor != ThisFrameActor) {
        LastFrameActor->UnHighlightActor();
        ThisFrameActor->HighlightActor();
    } else if (LastFrameActor && !ThisFrameActor) {
        LastFrameActor->UnHighlightActor();
    }
}


