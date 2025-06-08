// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "DefaultGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {
    check(AttributeInfo);
    auto DefaultAttributeSet = CastChecked<UDefaultAttributeSet>(AttributeSet);

    AttributeInfo->AttributeInfoMap[FDefaultGameplayTags::Attributes_Primary_Intelligence];

    for(auto& Pair : AttributeInfo->AttributeInfoMap) {
        auto Attribute = DefaultAttributeSet->AttributeGetMap[Pair.Key]();
        auto& AttributeChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute);
        AttributeChangedDelegate.AddLambda(
            [this, Pair](const FOnAttributeChangeData& Data) mutable {
                Pair.Value.AttributeValue = Data.NewValue;
                OnAttributeInfoChangedDelegate.Broadcast(Pair.Value);
            }
        );


        Pair.Value.AttributeValue = Attribute.GetNumericValue(DefaultAttributeSet);
        OnAttributeInfoChangedDelegate.Broadcast(Pair.Value);
    }

}

void UAttributeMenuWidgetController::BroadcastInitialValues() {
    check(AttributeInfo);
    auto DefaultAttributeSet = CastChecked<UDefaultAttributeSet>(AttributeSet);

    for(auto& Pair : AttributeInfo->AttributeInfoMap) {
        auto Attribute = DefaultAttributeSet->AttributeGetMap[Pair.Key]();
        Pair.Value.AttributeValue = Attribute.GetNumericValue(DefaultAttributeSet);
        OnAttributeInfoChangedDelegate.Broadcast(Pair.Value);
    }

}
