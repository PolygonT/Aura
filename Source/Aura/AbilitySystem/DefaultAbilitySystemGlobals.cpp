// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultAbilitySystemGlobals.h"
#include "DefaultAbilityTypes.h"

FGameplayEffectContext *
UDefaultAbilitySystemGlobals::AllocGameplayEffectContext() const {
    return new FDefaultGameplayEffectContext();
}
