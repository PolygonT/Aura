// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "DefaultAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilityWidgetController;
struct FGameplayEffectContextHandle;
struct FDefaultGameplayEffectContext;
struct FTaggedMontage;

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary")
    static UOverlayWidgetController*
    GetOverlayWidgetController(const UObject *WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary")
    static UAttributeMenuWidgetController *
    GetAttributeMenuWidgetController(const UObject *WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary")
    static UAbilityWidgetController *
    GetAbilityWidgetController(const UObject *WorldContextObject);

    // UFUNCTION()
    // static void InitializeDefaultAttributes(const UObject *WorldContextObject,
    //                                         ECharacterClass CharacterClass,
    //                                         float Level);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary")
    static UCharacterClassInfo *
    GetCharacterClassInfo(const UObject *WorldContextObject);


    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibarary|GameplayEffects")
    static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibarary|GameplayEffects")
    static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary|GameplayEffects")
    static void
    SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle &ContextHandle,
                    bool bInIsBlockedHit);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary|GameplayEffects")
    static void
    SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle &ContextHandle,
                     bool bInIsCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary|GameplayMechanics")
    static void
    GetLivePlayersWithinRadius(const UObject *WorldContextObject,
                               TArray<AActor *> &OutOverlappingActors,
                               const TArray<AActor *> &ActorsToIgnore,
                               float Radius, const FVector &SphereOrigin);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary|CollectionUtils")
    static FTaggedMontage GetRandomElementOfTagArr(TArray<FTaggedMontage> arr);

    UFUNCTION(BlueprintCallable, Category = "BlueprintDebugLibaray")
    static void DrawDebugCircleArcWrapper(
        const UObject *WorldContextObject, const FVector &Center, float Radius,
        const FVector &Direction, float AngleWidth, int32 Segments,
        const FColor &Color, bool PersistentLines = false,
        float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibarary|VectorUtils")
    static TArray<FVector> GetSpawnVectorsByNums(FVector ForwardVector,
                                                 float MaxiumnAngle,
                                                 float BestAngle, int32 Nums);

  private:
    static const FDefaultGameplayEffectContext*
    GetDefaultContext(const FGameplayEffectContextHandle &ContextHandle);

    static FDefaultGameplayEffectContext*
    GetDefaultContextNonConst(FGameplayEffectContextHandle &ContextHandle);
};
