// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "DefaultAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibarary")
    static UOverlayWidgetController*
    GetOverlayWidgetController(const UObject *WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AbilitySystemLibarary")
    static UAttributeMenuWidgetController *
    GetAttributeMenuWidgetController(const UObject *WorldContextObject);

    // UFUNCTION()
    // static void InitializeDefaultAttributes(const UObject *WorldContextObject,
    //                                         ECharacterClass CharacterClass,
    //                                         float Level);
};
