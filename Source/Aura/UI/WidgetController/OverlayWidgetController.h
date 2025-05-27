// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

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

    void HealthChanged(const FOnAttributeChangeData& Data);

    void MaxHealthChanged(const FOnAttributeChangeData& Data);

    void ManaChanged(const FOnAttributeChangeData& Data);

    void MaxManaChanged(const FOnAttributeChangeData& Data);

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnHealthChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnMaxHealthChangedSignature OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnManaChangedSignature OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnMaxManaChangedSignature OnMaxManaChanged;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetData")
    TObjectPtr<UDataTable> MessageWidgetDataTable;
};
