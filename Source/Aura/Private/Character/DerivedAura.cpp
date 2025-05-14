// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DerivedAura.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/DefaultPlayerState.h"

ADerivedAura::ADerivedAura() {
    // 移动时根据移动方向旋转Mesh的角度
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 700.f, 0.f);
    // GetCharacterMovement()->bConstrainToPlane = true;

    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;

    // PlayerState = CreateDefaultSubobject<ADefaultPlayerState>("PlayerState");
}


void ADerivedAura::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);

    // Init Ability actor info for the Server
    auto AuraPlayerState = GetPlayerStateChecked<ADefaultPlayerState>();
    AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AttributeSet = AuraPlayerState->GetAttributeSet();
}

void ADerivedAura::OnRep_PlayerState() {
    Super::OnRep_PlayerState();

    auto AuraPlayerState = GetPlayerStateChecked<ADefaultPlayerState>();
    AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AttributeSet = AuraPlayerState->GetAttributeSet();
}
