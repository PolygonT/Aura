// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "DefaultGameplayTags.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/HitResult.h"
#include "GameFramework/Pawn.h"
#include "Input/DefaultInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"


#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADefaultPlayerController::ADefaultPlayerController() {
    bReplicates = true;

    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ADefaultPlayerController::BeginPlay() {
    Super::BeginPlay();

    check(InputMappingContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem) {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData {};
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void ADefaultPlayerController::SetupInputComponent() {
    check(InputConfig);

    Super::SetupInputComponent();
    
    auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Move);
    EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ADefaultPlayerController::ShiftPressed);
    EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ADefaultPlayerController::ShiftReleased);


    for(const auto& Pair : InputConfig->AbilityInputActionMap) {
        EnhancedInputComponent->BindAction(Pair.Value, ETriggerEvent::Triggered, this, &ADefaultPlayerController::AbilityInputTagHeld, Pair.Key);
        EnhancedInputComponent->BindAction(Pair.Value, ETriggerEvent::Started, this, &ADefaultPlayerController::AbilityInputTagPressed, Pair.Key);
        EnhancedInputComponent->BindAction(Pair.Value, ETriggerEvent::Completed, this, &ADefaultPlayerController::AbilityInputTagReleased, Pair.Key);
    }

    // EnhancedInputComponent->BindActionFromAbilities(
    //     InputConfig,
    //     this,
    //     &ThisClass::AbilityInputTagHeld,
    //     &ThisClass::AbilityInputTagPressed,
    //     &ThisClass::AbilityInputTagReleased
    // );

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

void ADefaultPlayerController::AutoRun() {
    if (!bAutoRunning) {
        return;
    }

    if (APawn *ControlledPawn = GetPawn()) {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
            ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
            ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        ;

        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination =
            (LocationOnSpline - CachedDestination).Length();

        if (DistanceToDestination <= AutoRunAcceptanceRadius) {
            bAutoRunning = false;
        }
    }
}

void ADefaultPlayerController::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);

  CursorTrace();

  AutoRun();
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

void ADefaultPlayerController::AbilityInputTagPressed(const FInputActionValue &value, const FGameplayTag Tag) {
    // if (GEngine) {
    //     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Tag.ToString());
    // }
    bTargeting = ThisFrameActor ? true : false;
    bAutoRunning = false;
}

void ADefaultPlayerController::AbilityInputTagReleased(const FInputActionValue &value, const FGameplayTag Tag) {
    if (!GetASC()) return;

    if (!Tag.MatchesTagExact(FDefaultGameplayTags::InputTags_LMB)) {
        GetASC()->AbilityInputTagReleased(Tag);
        return;
    }

    GetASC()->AbilityInputTagReleased(Tag);

    if (!bTargeting && !bShiftKeyDown) {
        APawn* ControlledPawn = GetPawn();
        if (FollowTime > ShortPressThreshold || !ControlledPawn) {
            return;
        }

        if (UNavigationPath* NavPath =
            UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination)) {
            Spline->ClearSplinePoints();

            for (const auto& PointLocation : NavPath->PathPoints) {
                Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
                DrawDebugSphere(GetWorld(), PointLocation, 8.f, 24, FColor::Green, false, 5.f);
            }

            if (NavPath->PathPoints.Num() > 0) {
                CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
            } else {
                // CachedDestination = ControlledPawn->GetActorLocation();
            }

            bAutoRunning = true;
        }
    }

    bTargeting = false;
    FollowTime = 0.f;

}

void ADefaultPlayerController::AbilityInputTagHeld(const FInputActionValue &value, const FGameplayTag Tag) {
    if (!GetASC()) return;

    if (!Tag.MatchesTagExact(FDefaultGameplayTags::InputTags_LMB) || bTargeting || bShiftKeyDown) {
        GetASC()->AbilityInputTagHeld(Tag);
        return;
    }

    FollowTime += GetWorld()->GetDeltaSeconds();
    FHitResult Hit;

    if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) {
        CachedDestination = Hit.Location;
    }

    if (APawn* ControlledPawn = GetPawn()) {
        const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
        ControlledPawn->AddMovementInput(WorldDirection);
    }
}

UDefaultAbilitySystemComponent *ADefaultPlayerController::GetASC() {
    if (!DefaultAbilitySystemComponent) {
        auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
        DefaultAbilitySystemComponent = CastChecked<UDefaultAbilitySystemComponent>(ASC);
    }

    return DefaultAbilitySystemComponent;
}

void ADefaultPlayerController::ShiftPressed() {
    bShiftKeyDown = true;
}

void ADefaultPlayerController::ShiftReleased() {
    bShiftKeyDown = false;
}

