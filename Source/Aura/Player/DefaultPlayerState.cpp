// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerState.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADefaultPlayerState::ADefaultPlayerState() {
    NetUpdateFrequency = 100.f;

    AbilitySystemComponent = CreateDefaultSubobject<UDefaultAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UDefaultAttributeSet>("AttributeSet");

}

UAbilitySystemComponent* ADefaultPlayerState::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}

void ADefaultPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {

    DOREPLIFETIME(ADefaultPlayerState, PlayerLevel);
}

void ADefaultPlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel) {
}

