// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayAbilitySpec.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
class UTexture2D;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag MessageTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Message {};

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UDefaultUserWidget> MessageWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Image;
};

USTRUCT(BlueprintType) 
struct FAbilityRow {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Tag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag CooldownTag;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitiesChangeSignature, FAbilityRow, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UDefaultWidgetController
{
	GENERATED_BODY()
	
public:
    virtual void BroadcastInitialValues() override;

    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnMaxManaChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
    FMessageWidgetRowSignature MessageWidgetRowDelegate;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnCooldownChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Abilities")
    FOnAbilitiesChangeSignature OnAbilityChanged;

    UFUNCTION()
    void OnAbilityChangedCallback(FText Name, FGameplayTag Tag, int32 Level, FGameplayTag CooldownTag);

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetData")
    TObjectPtr<UDataTable> MessageWidgetDataTable;

    template<typename T>
    T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

private:
    
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) {
    return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
