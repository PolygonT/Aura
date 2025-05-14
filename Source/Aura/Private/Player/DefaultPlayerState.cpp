// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DefaultPlayerState.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"

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
