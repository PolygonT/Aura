// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "Engine/Engine.h"
#include "DefaultGameplayTags.h"

UDefaultAssetManager &UDefaultAssetManager::Get() {
    check(GEngine);

    auto DefaultAssetManager = Cast<UDefaultAssetManager>(GEngine->AssetManager);
    return *DefaultAssetManager;
}

void UDefaultAssetManager::StartInitialLoading() {
    Super::StartInitialLoading();

    // FDefaultGameplayTags::InitNativeGameplayTags();

    // This is required to use Target Data
    UAbilitySystemGlobals::Get().InitGlobalData();
}
