// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "AbilityWidgetController.generated.h"

USTRUCT(BlueprintType) 
struct FAbilityRow {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Tag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Level {1};

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag CooldownTag;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitiesChangeSignature, FAbilityRow, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAbilityWidgetController : public UDefaultWidgetController
{
	GENERATED_BODY()
	
public:
    virtual void BroadcastInitialValues() override;

    virtual void BindCallbacksToDependencies() override;

    UFUNCTION(BlueprintCallable)
    void LevelUpAbilityWithTag(FGameplayTag Tag);

    UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
    FOnAbilitiesChangeSignature OnAbilityChanged;

    UFUNCTION()
    void OnAbilityChangedCallback(FText Name, FGameplayTag Tag, int32 Level,
                                  FGameplayTag CooldownTag);
};
