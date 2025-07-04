#include "GameplayAbilityUtils.h"

TUniquePtr<FGameplayEffectSpecHandle> GameplayAbilityUtils::ConstructEffectSpec(
    UObject *SourceObject, AActor *TargetActor,
    TSubclassOf<UGameplayEffect> GamePlayEffectClass, float EffectLevel) {

    auto TargetAbilitySystemComponent =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    if (!TargetAbilitySystemComponent)
        return nullptr;

    check(GamePlayEffectClass);

    auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();

    EffectContextHandle.AddSourceObject(SourceObject);

    return MakeUnique<FGameplayEffectSpecHandle>(TargetAbilitySystemComponent->MakeOutgoingSpec(
        GamePlayEffectClass, EffectLevel, EffectContextHandle));
}

TUniquePtr<FGameplayEffectSpecHandle> GameplayAbilityUtils::ConstructEffectSpec(
    UObject *SourceObject, UAbilitySystemComponent *SourceASC,
    UAbilitySystemComponent *TargetASC,
    TSubclassOf<UGameplayEffect> GamePlayEffectClass, float EffectLevel) {

    check(GamePlayEffectClass);

    auto EffectContextHandle = SourceASC->MakeEffectContext();

    EffectContextHandle.AddSourceObject(SourceObject);

    return MakeUnique<FGameplayEffectSpecHandle>(TargetASC->MakeOutgoingSpec(
        GamePlayEffectClass, EffectLevel, EffectContextHandle));
}

