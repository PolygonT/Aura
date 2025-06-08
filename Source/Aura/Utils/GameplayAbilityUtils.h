#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

namespace GameplayAbilityUtils {

TUniquePtr<FGameplayEffectSpecHandle>
ConstructEffectSpec(UObject *SourceObject, AActor *TargetActor,
                    TSubclassOf<UGameplayEffect> GamePlayEffectClass,
                    float EffectLevel);
}
