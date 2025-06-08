// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DefaultWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FDefaultAttributeInfo;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChangeSignature, const FDefaultAttributeInfo&, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UDefaultWidgetController
{
	GENERATED_BODY()
	
public:

    virtual void BindCallbacksToDependencies() override;

    virtual void BroadcastInitialValues() override;

protected:

    UPROPERTY(BlueprintAssignable)
    FOnAttributeInfoChangeSignature OnAttributeInfoChangedDelegate;

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAttributeInfo> AttributeInfo;
};
