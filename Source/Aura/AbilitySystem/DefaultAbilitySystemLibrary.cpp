// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "DefaultAbilityTypes.h"
#include "Engine/Engine.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Game/DefaultGameModeBase.h"
#include "GameplayEffectTypes.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DefaultPlayerState.h"
#include "UI/HUD/DefaultHUD.h"
#include "UI/WidgetController/DefaultWidgetController.h"

UOverlayWidgetController *
UDefaultAbilitySystemLibrary::GetOverlayWidgetController(
    const UObject *WorldContextObject) {
    auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (!PC) {
        return nullptr;
    }

    auto DefaultHUD = Cast<ADefaultHUD>(PC->GetHUD());

    if (!DefaultHUD) {
        return nullptr;
    }

    auto DefaultPlayerState = PC->GetPlayerState<ADefaultPlayerState>();

    auto AbilitySystemComponent = DefaultPlayerState->GetAbilitySystemComponent();

    auto AttributeSet = DefaultPlayerState->GetAttributeSet();

    const auto Params = 
        FWidgetControllerParams(PC, DefaultPlayerState, AbilitySystemComponent, AttributeSet);
    return DefaultHUD->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController *
UDefaultAbilitySystemLibrary::GetAttributeMenuWidgetController(
    const UObject *WorldContextObject) {
    auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (!PC) {
        return nullptr;
    }

    auto DefaultHUD = Cast<ADefaultHUD>(PC->GetHUD());

    if (!DefaultHUD) {
        return nullptr;
    }

    auto DefaultPlayerState = PC->GetPlayerState<ADefaultPlayerState>();

    auto AbilitySystemComponent = DefaultPlayerState->GetAbilitySystemComponent();

    auto AttributeSet = DefaultPlayerState->GetAttributeSet();

    const auto Params = 
        FWidgetControllerParams(PC, DefaultPlayerState, AbilitySystemComponent, AttributeSet);
    return DefaultHUD->GetAttributeMenuWidgetController(Params);

}

// void UDefaultAbilitySystemLibrary::InitializeDefaultAttributes(
//     const UObject *WorldContextObject, ECharacterClass CharacterClass,
//     float Level) {
//     
//     auto GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
//
//     if (!GameMode) { return; }
//
//     auto ClassInfo = GameMode->CharacterClassInfo;
//     auto ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
// }

UCharacterClassInfo *UDefaultAbilitySystemLibrary::GetCharacterClassInfo(
    const UObject *WorldContextObject) {
    auto GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

    if (!GameMode) {
        return nullptr;
    }

    return GameMode->CharacterClassInfo;
}

bool UDefaultAbilitySystemLibrary::IsBlockedHit(
    const FGameplayEffectContextHandle &ContextHandle) {

    if (const FDefaultGameplayEffectContext* DefaultContext = GetDefaultContext(ContextHandle)) {
        return DefaultContext->IsBlockedHit();
    }

    return false;

}

bool UDefaultAbilitySystemLibrary::IsCriticalHit(
    const FGameplayEffectContextHandle &ContextHandle) {

    if (const FDefaultGameplayEffectContext* DefaultContext = GetDefaultContext(ContextHandle)) {
        return DefaultContext->IsCriticalHit();
    }

    return false;
}

void UDefaultAbilitySystemLibrary::SetIsBlockedHit(
    FGameplayEffectContextHandle &ContextHandle, bool bInIsBlockedHit) {

    if (FDefaultGameplayEffectContext* DefaultContext = GetDefaultContextNonConst(ContextHandle)) {
        DefaultContext->SetIsBlockedHit(bInIsBlockedHit);
    }
}

void UDefaultAbilitySystemLibrary::SetIsCriticalHit(
    FGameplayEffectContextHandle &ContextHandle, bool bInIsCriticalHit) {

    if (FDefaultGameplayEffectContext* DefaultContext = GetDefaultContextNonConst(ContextHandle)) {
        DefaultContext->SetIsCriticalHit(bInIsCriticalHit);
    }
}

const FDefaultGameplayEffectContext*
UDefaultAbilitySystemLibrary::GetDefaultContext(
    const FGameplayEffectContextHandle &ContextHandle) {

    return static_cast<const FDefaultGameplayEffectContext*>(ContextHandle.Get());
}

FDefaultGameplayEffectContext *
UDefaultAbilitySystemLibrary::GetDefaultContextNonConst(
    FGameplayEffectContextHandle &ContextHandle) {

    return static_cast<FDefaultGameplayEffectContext*>(ContextHandle.Get());
}

void UDefaultAbilitySystemLibrary::GetLivePlayersWithinRadius(
    const UObject *WorldContextObject, TArray<AActor *> &OutOverlappingActors,
    const TArray<AActor *> &ActorsToIgnore, float Radius,
    const FVector &SphereOrigin) {

    FCollisionQueryParams SphereParams {};

    if (ActorsToIgnore.Num() > 0) {
        SphereParams.AddIgnoredActors(ActorsToIgnore);
    }

    TArray<FOverlapResult> Overlaps;

    if (const UWorld* World = 
        GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
        World->OverlapMultiByObjectType(
            Overlaps, SphereOrigin, FQuat::Identity,
            FCollisionObjectQueryParams{
                FCollisionObjectQueryParams::InitType::AllDynamicObjects},
            FCollisionShape::MakeSphere(Radius), SphereParams);

        for (const FOverlapResult &Overlap : Overlaps) {
            AActor *Actor = Overlap.GetActor();
            if (Actor->Implements<UCombatInterface>() &&
                !ICombatInterface::Execute_IsDead(Actor)) {
                OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvtar(Actor));
            }
        }
    }


}

// TODO 6 不返回引用会造成额外复制，但返回引用会闪退，非法内存访问
FTaggedMontage UDefaultAbilitySystemLibrary::GetRandomElementOfTagArr(
    TArray<FTaggedMontage> arr) {

    int32 arrSize = arr.Num();
    check(arrSize != 0);

    if (arrSize == 1) {
        return arr[0];
    }

    int32 randomIndex = FMath::RandRange(0, arrSize - 1);
    return arr[randomIndex];

}
